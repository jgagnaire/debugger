#include <iostream>
#include <string>
#include <map>

#include <sys/types.h>
#include <libdwarf.h>
#include <unistd.h>
#include <string.h>
#include <dwarf.h>
#include <elf.h>

#include "gueuldeboua.hh"

extern int file_fd;
extern Elf64_Ehdr header;
extern Elf64_Shdr *section_header_table;
extern long save_strtab_idx;

static std::map<std::string, Dwarf_Addr> local_varaddr_map;

static Dwarf_Addr get_var_addr(Dwarf_Die die, int *tmp_error)
{
  Dwarf_Error error;
  Dwarf_Attribute *attrs;
  Dwarf_Addr addr;
  Dwarf_Signed attrcount;

  if (::dwarf_attrlist(die, &attrs, &attrcount, &error) != DW_DLV_OK)
    {
      std::cout << "error in dwarf_attrlist()" << std::endl;
      *tmp_error = -1;
      return (-1);
    }
  for (long i = 0; i < attrcount; ++i)
    {
      Dwarf_Half attrcode;

      if (::dwarf_whatattr(attrs[i], &attrcode, &error) != DW_DLV_OK)
        {
	  std::cout << "error in dwarf_whatattr()" << std::endl;
	  *tmp_error = -1;
	  return (-1);
	}
      if (attrcode == DW_AT_location)
	{
	  ::dwarf_formaddr(attrs[i], &addr, 0);
	  return (addr);
	}
    }
  *tmp_error = 0;
  return (addr);
}

static void get_die_data(Dwarf_Debug dbg, Dwarf_Die die, int level)
{
  char *name = 0;
  int tmp_error = -1;
  Dwarf_Error error = 0;
  Dwarf_Half tag = 0;
  Dwarf_Addr tmp = 0;
  const char *tagname = 0;

  switch (::dwarf_diename(die, &name, &error))
    {
    case DW_DLV_NO_ENTRY:
      return ;
    case DW_DLV_ERROR:
      std::cout << "dwarf_diename() failed, at level " << level << std::endl;
      return ;
    }
  if (::dwarf_tag(die, &tag, &error) != DW_DLV_OK)
    {
      std::cout << "dwarf_tag() failed, at level " << level << std::endl;
      return ;
    }
  if (::dwarf_get_TAG_name(tag, &tagname) != DW_DLV_OK)
    {
      std::cout << "dwarf_get_TAG_name() failed, at level " << level << std::endl;
      return ;
    }
  if (::strcmp(tagname, "DW_TAG_variable"))
    {
      ::dwarf_dealloc(dbg, name, DW_DLA_STRING);
      return ;
    }
  tmp = get_var_addr(die, &tmp_error);
  if (tmp_error == -1)
    return ;
  local_varaddr_map[std::string(name)] = tmp;
  std::cout << "LE NOM ET L'ADRESSE DE LA VARIABLE: [" << name << " || " << tmp << "]" << std::endl;
}

static int get_die_and_siblings(Dwarf_Debug dbg, Dwarf_Die in_die, int level)
{
  Dwarf_Die cur_die = in_die;
  Dwarf_Die child = 0;
  Dwarf_Error error;

  get_die_data(dbg, in_die, level);
  while (1)
    {
      Dwarf_Die sib_die;

      switch (::dwarf_child(cur_die, &child, &error))
	{
	case DW_DLV_ERROR:
	  std::cout << "dwarf_child() failed" << std::endl;
	  return (-1);
	case DW_DLV_OK:
	  get_die_and_siblings(dbg, child, level + 1);
	  break ;
	}
      switch (::dwarf_siblingof(dbg, cur_die, &sib_die, &error))
	{
	case DW_DLV_NO_ENTRY:
	  return (0);
	case DW_DLV_ERROR:
	  std::cout << "dwarf_siblingof() failed" << std::endl;
	  return (-1);
	case DW_DLV_OK:
	  if (cur_die != in_die)
	    ::dwarf_dealloc(dbg, cur_die, DW_DLA_DIE);
	  break ;
	}
      cur_die = sib_die;
    }
}

static int read_cu_list(Dwarf_Debug dbg)
{
  Dwarf_Error error = 0;
  Dwarf_Half version_stamp = 0, address_size = 0;
  Dwarf_Unsigned cu_header_length = 0, abbrev_offset = 0, next_cu_header = 0;

  while (1)
    {
      Dwarf_Die cu_die = 0;

      switch (::dwarf_next_cu_header(dbg, &cu_header_length,
				     &version_stamp, &abbrev_offset,
				     &address_size, &next_cu_header,
				     &error))
	{
	case DW_DLV_ERROR:
	  std::cout << "dwarf_next_cu_header() failed" << std::endl;
	  return (-1);
	case DW_DLV_NO_ENTRY:
	  return (0);
	}
      switch (::dwarf_siblingof(dbg, 0, &cu_die, &error))
	{
	case DW_DLV_ERROR:
	  std::cout << "dwarf_sibling_of() failed" << std::endl;
	  return (-1);
	case DW_DLV_NO_ENTRY:
	  std::cout << "no entry in dwarf_siblingof()" << std::endl;
	  return (-1);
	}
      if (get_die_and_siblings(dbg, cu_die, 0) == -1)
	return (-1);
      ::dwarf_dealloc(dbg, cu_die, DW_DLA_DIE);
    }
  return (0);
}

int get_localvar_addr(std::string const &varname,
		      Elf64_Word *varaddr,
		      unsigned long *varsize)
{
  Dwarf_Debug dbg = 0;
  Dwarf_Error error;

  (void)varname; (void)varaddr; (void)varsize;
  if (::lseek(file_fd, 0, SEEK_SET) == -1
      || ::dwarf_init(file_fd, DW_DLC_READ, 0, 0, &dbg, &error) != DW_DLV_OK
      || read_cu_list(dbg) == -1)
    {
      std::cout << "fichier invalide" << std::endl;
      return (-1);
    }
  (void)::dwarf_finish(dbg, &error);
  return (0);
}
