
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <iostream>

int file_fd;

unsigned long get_sym_addr(std::string const &sym_name, int *error)
{
  fstat();
  (void)sym_name,
  (void)error;
  *error = 0;
  return 0;
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
    {
      std::cout << "file le nom de la fonction à laquelle tu veux breaker" << std::endl;
      return ;
    }

  int error;

  symaddr = get_sym_addr(cmd.substr(i), &error);  
  (void)symaddr;
  if (error == -1)
    {
      std::cout << "donne une fonction qui existe, ça marchera mieux" << std::endl;
      return ;
    }
  //set_breakpoint(symaddr);
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
    {
      std::cout << USAGE << std::endl;
      return -1;
    }

  if ((file_fd = ::open(b[1], O_RDONLY, 0644)) == -1)
    {
      std::cout << "impossible d'ouvrir le fichier" << std::endl;
      return -1;
    }

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
	  return 0;
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
  std::cout << std::endl;
  return 0;
}
