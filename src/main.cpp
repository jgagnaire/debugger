
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <elf.h>
#include <map>

int file_fd;
Elf64_Ehdr header;
Elf64_Shdr *section_header_table;
std::map<std::string, Elf64_Sym> symaddr_map;

Elf64_Word get_sym_addr(std::string const &sym_name, int *error)
{
  if (!symaddr_map.empty())
    goto ret_label;

  if (::read(file_fd, &header, sizeof(Elf64_Ehdr)) <= 0
      || !header.e_shoff
      || ::lseek(file_fd, header.e_shoff, SEEK_SET) == -1
      || !(section_header_table = (Elf64_Shdr *)::calloc(header.e_shnum, sizeof(Elf64_Shdr))))
    return (std::cout << "fichier invalide ou impossible d'allouer de la memoire" << std::endl, *error = -1, 0);

  long save_symtab_idx, save_strtab_idx;

  save_symtab_idx = 0, save_strtab_idx = 0;
  for (unsigned long i = 0;i < header.e_shnum;++i)
    {
      if (::read(file_fd, &section_header_table[i], sizeof(Elf64_Shdr)) <= 0)
	return (std::cout << "fichier invalide" << std::endl, *error = -1, 0);
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
      

  char *strtab;

  if (!(strtab = (char *)::calloc(section_header_table[save_strtab_idx].sh_size, 1))
      || ::lseek(file_fd, section_header_table[save_strtab_idx].sh_offset, SEEK_SET) == -1
      || ::read(file_fd, strtab, section_header_table[save_strtab_idx].sh_size) <= 0)
    return (std::cout << "fichier invalide ou impossible d'allouer de la memoire" << std::endl, *error = -1, 0);


  Elf64_Sym *symtab;

  if (save_symtab_idx == -1 || save_strtab_idx == -1
      || ::lseek(file_fd, section_header_table[save_symtab_idx].sh_offset, SEEK_SET) == -1
      || !(symtab = (Elf64_Sym *)::calloc(section_header_table[save_symtab_idx].sh_size / sizeof(Elf64_Sym), sizeof(Elf64_Sym))))
    return (std::cout << "fichier invalide ou impossible d'allouer de la memoire" << std::endl, *error = -1, 0);

  for (unsigned long i = 0;i < (section_header_table[save_symtab_idx].sh_size / sizeof(Elf64_Sym));++i)
    {
      if (::read(file_fd, &symtab[i], sizeof(Elf64_Sym)) <= 0)
	return (std::cout << "fichier invalide" << std::endl, *error = -1, 0);
      symaddr_map[std::string(&strtab[symtab[i].st_name])] = symtab[i];
    }
  ::free(symtab);
  ::free(strtab);

 ret_label:
  try {
    return (*error = 0, symaddr_map.at(sym_name).st_value);
  }
  catch (std::exception const &) {
    return (std::cout << "file une fonction qui existe, ca marchera probablement un peu mieux" << std::endl, *error = -1, 0);
  }
}



void run_fct(std::string const &)
{
  std::cout << "commande: [run] (et lekudlashat aussi)" << std::endl;
}

void set_fct(std::string const &)
{
  std::cout << "commande: [set] (et leusex aussi)" << std::endl;
}

void break_fct(std::string const &cmd)
{
  unsigned long i, symaddr;

  if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() - 1)
    return ((void)(std::cout << "file le nom de la fonction à laquelle tu veux breaker" << std::endl));

  int error;

  std::cout << "l'adresse à laquelle on veut mettre un point de cassure est "
	    << std::hex
	    << (symaddr = get_sym_addr(cmd.substr(i + 1), &error))
	    << std::endl;
  if (error == -1)
    return ;
  // set_breakpoint(symaddr);
}

void delete_fct(std::string const &)
{
  std::cout << "commande: [delete] (et lekouy aussi)" << std::endl;
}

void print_fct(std::string const &)
{
  std::cout << "commande: [print] (et leuzobe aussi)" << std::endl;
}

void printreg_fct(std::string const &)
{
  std::cout << "commande: [printreg] (et leuphyon aussi)" << std::endl;
}

void setreg_fct(std::string const &)
{
  std::cout << "commande: [setreg] (et leuku aussi)" << std::endl;
}

void backtrace_fct(std::string const &)
{
  std::cout << "commande: [backtrace] (et leuku aussi)" << std::endl;
}


#define CMD_NB 11

#define RUN_CMD 0
#define KILL_CMD 1
#define QUIT_CMD 2
#define SET_CMD 3
#define BREAK_CMD 4
#define DELETE_CMD 5
#define PRINT_CMD 6
#define PRINTREG_CMD 7
#define SETREG_CMD 8
#define BACKTRACE_CMD 9
#define HELP_CMD 10

#define PROMPT "[GueulDeBoua] "
#define USAGE "Usage: ./GueulDeBoua programme_a_deboguer"
#define CNF "commande inexistante, tape help si t'es perdu"
#define HELP "commandes possibles: run, kill, quit, set, break, delete, print, printreg, setreg, backtrace, help"

int         main(int, char *b[])
{
  if (!b[1])
    return (std::cout << USAGE << std::endl, -1);

  if ((file_fd = ::open(b[1], O_RDONLY, 0644)) == -1)
    return (std::cout << "impossible d'ouvrir le fichier" << std::endl, -1);

  static const std::string cmd_tab[CMD_NB] = {"run", "kill", "quit", "set",
					      "break", "delete", "print",
					      "printreg", "setreg", "backtrace",
					      "help"};

  for (std::string tmp;std::cout << PROMPT and std::getline(std::cin, tmp);)
    {
      if (tmp.empty())
	continue ;

      unsigned long a, i = tmp.find(" ");
      std::string cmd = tmp.substr(0, i == std::string::npos ? tmp.size() : i);

      for (a = 0;a < CMD_NB and cmd.compare(cmd_tab[a]);++a);

      switch (a)
	{
	case RUN_CMD:
	  run_fct(tmp);
	  break ;
	case QUIT_CMD:
	  return (0);
	case KILL_CMD:
	  set_fct(tmp);
	  break ;
	case BREAK_CMD:
	  break_fct(tmp);
	  break ;
	case DELETE_CMD:
	  delete_fct(tmp);
	  break ;
	case PRINT_CMD:
	  print_fct(tmp);
	  break ;
	case PRINTREG_CMD:
	  printreg_fct(tmp);
	  break ;
	case SETREG_CMD:
	  setreg_fct(tmp);
	  break ;
	case BACKTRACE_CMD:
	  backtrace_fct(tmp);
	  break ;
	case HELP_CMD:
	  std::cout << HELP << std::endl;
	  break ;
	default:
	  std::cerr << CNF << std::endl;
	  break ;
	}
    }
  return (std::cout << std::endl, 0);
}
