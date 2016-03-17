#include <iostream>
#include <string>
#include <map>

#include <sys/types.h>
#include <libdwarf.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dwarf.h>
#include <elf.h>

#include "gueuldeboua.hh"

extern int file_fd;
extern Elf64_Ehdr header;
extern Elf64_Shdr *section_header_table;
extern long save_strtab_idx;

static std::map<std::string, func_struct> die_map;

void create_func_struct(Dwarf_Die die, char *name)
{
  func_struct *x;
  Dwarf_Error error;
  Dwarf_Signed attr_nb; 
  Dwarf_Attribute *attrlist; 
  Dwarf_Addr func_addr;
  int ret = 0;

  if (::dwarf_attrlist(die, &attrlist, &attr_nb, &error) != DW_DLV_OK)
    return ;
  for (long i = 0;i < attr_nb;++i)
    {
      Dwarf_Half attrcode;

      if ((ret = ::dwarf_whatattr(attrlist[i], &attrcode, &error)) == DW_DLV_OK
	  && attrcode == DW_AT_low_pc)
	{
	  x = new func_struct();
	  ::dwarf_formaddr(attrlist[i], &func_addr, 0);
	  x->func_addr = func_addr;
	  die_map[std::string(name)] = *x;
	  return ;
	}
      else if (ret == DW_DLV_ERROR)
	{
	  std::cout << "dwarf_whatattr() failed" << std::endl;
	  return ;
	}
    }
}

static void get_die_data(Dwarf_Debug dbg, Dwarf_Die die, int level)
{
  char *name = 0;
  int tmp_error = -1;
  Dwarf_Error error = 0;
  Dwarf_Half tag = 0;

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
  switch (tag)
    {
    case DW_TAG_subprogram:
      create_func_struct(die, name);
      break ;
    case DW_TAG_variable:
      std::cout << "variable name: " << name << " level: " << level << std::endl;
      break ;
    case DW_TAG_formal_parameter:
      std::cout << "parameter name: " << name << " level: " << level << std::endl;
      break ;    
    }
}

static int get_die_and_siblings(Dwarf_Debug dbg, Dwarf_Die in_die, int level)
{
  Dwarf_Die cur_die = in_die;
  Dwarf_Die child = 0;
  Dwarf_Error error;

  while (1)
    {
      get_die_data(dbg, cur_die, level);
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
	}
      if (cur_die != in_die)
	::dwarf_dealloc(dbg, cur_die, DW_DLA_DIE);
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
