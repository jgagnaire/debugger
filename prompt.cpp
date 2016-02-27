
#include <string>
#include <iostream>

void run_fct(std::string const &)
{
  std::cout << "commande: [run] (et lekudlashat aussi)" << std::endl;
}

void set_fct(std::string const &)
{
  std::cout << "commande: [set] (et leusex aussi)" << std::endl;
}

void break_fct(std::string const &)
{
  std::cout << "commande: [break] (et leudayrch aussi)" << std::endl;
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
#define USAGE "Usage: ./GueulDeBoua program_to_debug"
#define CNF "commande inexistante, tape help si t'es perdu"


int main(int, char **b)
{
  if (!b[1])
    {
      std::cout << "file un argument salbatarre" << std::endl;
      return -1;
    }

  static const std::string cmd_tab[CMD_NB] = {"run", "kill", "quit", "set",
					      "break", "delete", "print",
					      "printreg", "setreg", "backtrace",
					      "help"};

  for (std::string tmp;std::cout << PROMPT and std::getline(std::cin, tmp);)
    {
      unsigned long a, i = tmp.find(" ");
      std::string cmd = tmp.substr(0, i == std::string::npos ? tmp.size() : i);

      for (a = 0;a < CMD_NB;++a)
	{
	  if (!cmd.compare(cmd_tab[a]))
	    break ;
	}

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
	  std::cout << USAGE << std::endl;
	  break ;
	default:
	  std::cerr << CNF << std::endl;
	  break ;
	}
    }
  return 0;
}
