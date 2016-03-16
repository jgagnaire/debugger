#include <iostream>
#include <string>

#include <sys/types.h>
#include <libdwarf.h>
#include <unistd.h>
#include <elf.h>

#include "gueuldeboua.hh"

extern int file_fd;
extern Elf64_Ehdr header;
extern Elf64_Shdr *section_header_table;
extern long save_strtab_idx;

int get_localvar_addr(std::string const &varname,
		      Elf64_Word *varaddr,
		      unsigned long *varsize)
{
  Dwarf_Debug ret;
  Dwarf_Half version_stamp, address_size;
  Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
  Dwarf_Error error;

  (void)varname; (void)varaddr; (void)varsize;
  if (::lseek(file_fd, 0, SEEK_SET) == -1
      || ::dwarf_init(file_fd, DW_DLC_READ, 0, 0, &ret, 0) != DW_DLV_OK)
    {
      std::cout << "fichier invalide" << std::endl;
      return (-1);
    }
  for (unsigned long cu_number = 0;;++cu_number)
    {
      switch (::dwarf_next_cu_header(ret, &cu_header_length,
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
    }
  return (0);
}
