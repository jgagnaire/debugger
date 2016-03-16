
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <elf.h>

#include <iostream>
#include <string>
#include <map>

extern int file_fd;
static std::map<std::string, Elf64_Sym> global_symaddr_map;
long save_strtab_idx;
long save_symtab_idx;

static char *get_strtab(Elf64_Shdr *section_header_table, int *error)
{
  char *strtab;

  if (!(strtab = (char *)::calloc(section_header_table[save_strtab_idx].sh_size, 1))
      || ::lseek(file_fd, section_header_table[save_strtab_idx].sh_offset, SEEK_SET) == -1
      || ::read(file_fd, strtab, section_header_table[save_strtab_idx].sh_size) <= 0)
    {
      std::cout << "fichier invalide ou impossible d'allouer de la memoire" << std::endl;
      *error = -1;
      return (0);
    }
  return (strtab);
}

static Elf64_Shdr *get_section_header_table(Elf64_Ehdr *header, int *error)
{
  Elf64_Shdr *section_header_table;

  if (!(section_header_table = (Elf64_Shdr *)::calloc(header->e_shnum, sizeof(Elf64_Shdr))))
    return (0);
  for (unsigned long i = 0;i < header->e_shnum;++i)
    {
      if (::read(file_fd, &section_header_table[i], sizeof(Elf64_Shdr)) <= 0)
        {
	  std::cout << "fichier invalide" << std::endl;
	  *error = -1;
	  return (0);
	}
      switch (section_header_table[i].sh_type)
        {
        case SHT_SYMTAB:
          save_symtab_idx = i;
          break ;
        case SHT_STRTAB:
          save_strtab_idx = i;
          break ;
        default :
          break ;
        }
    }
  return (section_header_table);
}

static int fill_header(Elf64_Ehdr *header, int *error)
{
  if (::read(file_fd, header, sizeof(Elf64_Ehdr)) <= 0
      || !header->e_shoff
      || ::lseek(file_fd, header->e_shoff, SEEK_SET) == -1)
    {
      std::cout << "fichier invalide" << std::endl;
      *error = -1;
      return (-1);
    }
  return (0);
}

static int get_symtab_and_fill_map(char *strtab,
				   Elf64_Shdr *section_header_table,
				   int *error)
{
  Elf64_Sym *symtab;

  if (::lseek(file_fd, section_header_table[save_symtab_idx].sh_offset, SEEK_SET) == -1
      || !(symtab = (Elf64_Sym *)::calloc(section_header_table[save_symtab_idx].sh_size
					  / sizeof(Elf64_Sym), sizeof(Elf64_Sym))))
    {
      std::cout << "fichier invalide ou impossible d'allouer de la memoire" << std::endl;
      *error = -1;
      return (-1);
    }
  for (unsigned long i = 0;
       i < (section_header_table[save_symtab_idx].sh_size / sizeof(Elf64_Sym));
       ++i)
    {
      if (::read(file_fd, &symtab[i], sizeof(Elf64_Sym)) <= 0)
        {
	  std::cout << "fichier invalide" << std::endl;
	  *error = -1;
	  return (-1);
	}
      global_symaddr_map[std::string(&strtab[symtab[i].st_name])] = symtab[i];
    }
  ::free(symtab);
  ::free(strtab);
  return (0);
}

Elf64_Word get_globalsym_addr(std::string const &sym_name, int *error)
{
  Elf64_Ehdr header;
  Elf64_Shdr *section_header_table;
  long save_symtab_idx, save_strtab_idx;
  char *strtab;
  
  if (!global_symaddr_map.empty())
    goto ret_label;
  save_symtab_idx = -1;
  save_strtab_idx = -1;
  if (fill_header(&header, error) == -1
      || !(section_header_table = get_section_header_table(&header, error))
      || save_symtab_idx == -1 || save_strtab_idx == -1
      || !(strtab = get_strtab(section_header_table, error))
      || get_symtab_and_fill_map(strtab, section_header_table, error))
    return (0);
 ret_label:
  try {
    *error = 0;
    return (global_symaddr_map.at(sym_name).st_value);
  }
  catch (...) {
    std::cout << "file une fonction qui existe, ca marchera probablement un peu mieux"
	      << std::endl;
    *error = -1;
    return (0);
  }
}
