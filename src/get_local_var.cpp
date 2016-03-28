#include <iostream>
#include <string>
#include <map>

#include <sys/types.h>
#include <libdwarf.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dwarf.h>
#include <elf.h>

#include "my_gdb.hh"

extern int file_fd;
extern Elf64_Ehdr header;
extern Elf64_Shdr *section_header_table;
extern long save_strtab_idx;

static std::map<std::string, func_struct *> die_map;

static func_struct *create_func_struct(Dwarf_Die die, char *name)
{
  func_struct *x;
  Dwarf_Error error;
  Dwarf_Signed attr_nb;
  Dwarf_Attribute *attrlist;
  int32_t ret = 0;

  if (::dwarf_attrlist(die, &attrlist, &attr_nb, &error) != DW_DLV_OK)
    return (0);
  x = new func_struct();
  x->func_addr = 0;
  x->func_frame_base = 0;
  for (long i = 0;i < attr_nb;++i)
    {
      Dwarf_Half attrcode;

      if ((ret = ::dwarf_whatattr(attrlist[i], &attrcode, &error)) == DW_DLV_OK)
	switch (attrcode)
	  {
	  case DW_AT_low_pc:
	    ::dwarf_formaddr(attrlist[i], &x->func_addr, 0);
	    break ;
	  case DW_AT_frame_base:
	    Dwarf_Ptr tmp = 0;
	    Dwarf_Half theform = 0, directform = 0;

            ::dwarf_whatform(attrlist[i], &theform, &error);
	    ::dwarf_whatform_direct(attrlist[i], &directform, &error);
	    if (theform == DW_FORM_exprloc)
	      {
		::dwarf_formexprloc(attrlist[i], &x->func_frame_base_bytes_nb, &tmp, &error);
		switch (x->func_frame_base_bytes_nb)
		  {
		  case 1:
		    x->func_frame_base = *((uint8_t *)tmp);
		    break ;
		  case 2:
		    x->func_frame_base = *((uint16_t *)tmp);
		    break ;
		  case 4:
		    x->func_frame_base = *((uint32_t *)tmp);
		    break ;
		  case 8:
		    x->func_frame_base = *((uint64_t *)tmp);
		    break ;
		  }
	      }
	    break ;
	  }
      else if (ret == DW_DLV_ERROR)
	{
	  std::cerr << "dwarf_whatattr() failed" << std::endl;
	  return (0);
	}
    }
  die_map[std::string(name)] = x;
  return (x);
}

static var_struct *create_var_struct(Dwarf_Die die)
{
  Dwarf_Error error = 0;
  Dwarf_Signed attr_nb; 
  Dwarf_Attribute *attrlist;
  int ret = 0;
  var_struct *x = 0;

  if (::dwarf_attrlist(die, &attrlist, &attr_nb, &error) != DW_DLV_OK)
    return (0);
  for (long i = 0;i < attr_nb;++i)
    {
      Dwarf_Half attrcode;

      if ((ret = ::dwarf_whatattr(attrlist[i], &attrcode, &error)) == DW_DLV_OK
	  && attrcode == DW_AT_location)
	{
	  Dwarf_Locdesc *llbuf = 0;
	  Dwarf_Locdesc **llbufarray = 0;
	  Dwarf_Signed nb_of_elements = 0;

	  if (!(x = (var_struct *)::calloc(1, sizeof(var_struct)))
	      || (ret = ::dwarf_loclist_n(attrlist[i], &llbufarray, &nb_of_elements, &error)) != DW_DLV_OK)
	    return (0);
	  for (long llent = 0;llent < nb_of_elements;++llent)
	    {
	      llbuf = llbufarray[llent];

	      for (long k = 0; k < llbuf->ld_cents; ++k) {
		switch (llbuf->ld_s[k].lr_atom)
		  {
		  case DW_OP_breg6:
		    x->rbp_offset = (Dwarf_Signed)llbuf->ld_s[k].lr_number;
		    break ;
		  case DW_OP_fbreg:
		    x->fbreg_offset = (Dwarf_Signed)llbuf->ld_s[k].lr_number;
		    break ;
		  }
	      }
	    }
	}
      else if (ret == DW_DLV_ERROR)
	{
	  std::cerr << "dwarf_whatattr() failed" << std::endl;
	  return (0);
	}
    }
  return (x);
}

static void get_die_data(Dwarf_Die die, int level)
{
  static func_struct *save_parent = 0;
  char *name = 0;
  Dwarf_Error error = 0;
  Dwarf_Half tag = 0;

  switch (::dwarf_diename(die, &name, &error))
    {
    case DW_DLV_NO_ENTRY:
      return ;
    case DW_DLV_ERROR:
      std::cerr << "dwarf_diename() failed, at level " << level << std::endl;
      return ;
    }
  if (::dwarf_tag(die, &tag, &error) != DW_DLV_OK)
    {
      std::cerr << "dwarf_tag() failed, at level " << level << std::endl;
      return ;
    }
  switch (tag)
    {
    case DW_TAG_subprogram:
      save_parent = create_func_struct(die, name);
      break ;
    case DW_TAG_formal_parameter:
      save_parent->params_map[std::string(name)] = create_var_struct(die);
      break ;
    case DW_TAG_variable:
      save_parent->variables_map[std::string(name)] = create_var_struct(die);
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
      get_die_data(cur_die, level);
      Dwarf_Die sib_die;

      switch (::dwarf_child(cur_die, &child, &error))
	{
	case DW_DLV_ERROR:
	  std::cerr << "dwarf_child() failed" << std::endl;
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
	  std::cerr << "dwarf_siblingof() failed" << std::endl;
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
	  std::cerr << "dwarf_next_cu_header() failed" << std::endl;
	  return (-1);
	case DW_DLV_NO_ENTRY:
	  return (0);
	}
      switch (::dwarf_siblingof(dbg, 0, &cu_die, &error))
	{
	case DW_DLV_ERROR:
	  std::cerr << "dwarf_sibling_of() failed" << std::endl;
	  return (-1);
	case DW_DLV_NO_ENTRY:
	  std::cerr << "no entry in dwarf_siblingof()" << std::endl;
	  return (-1);
	}
      if (get_die_and_siblings(dbg, cu_die, 0) == -1)
	return (-1);
      ::dwarf_dealloc(dbg, cu_die, DW_DLA_DIE);
    }
  return (0);
}

func_struct *get_func_struct(std::string const &fctname)
{
  Dwarf_Debug dbg = 0;
  Dwarf_Error error;

  if (!die_map.empty())
    goto ret_label;
  if (::lseek(file_fd, 0, SEEK_SET) == -1
      || ::dwarf_init(file_fd, DW_DLC_READ, 0, 0, &dbg, &error) != DW_DLV_OK
      || read_cu_list(dbg) == -1)
    {
      std::cerr << "invalid file" << std::endl;
      return (0);
    }
  ::dwarf_finish(dbg, &error);

 ret_label:
  try {
    return (die_map.at(fctname));
  }
  catch (...) {
    std::cerr << "invalid parameters" << std::endl;
    return (0);
  }
}
