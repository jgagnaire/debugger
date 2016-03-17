
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <stdexcept>
#include <string>
#include <iostream>

#include "gueuldeboua.hh"
#include "debugger.hh"

int file_fd;
int _child_pid;

int main(int, char **av)
{
  static const std::string cmd_tab[CMD_NB] =
    {"run", "kill", "quit", "set",
     "break", "delete", "print",
     "printreg", "setreg", "backtrace",
     "help"};

  if (!av[1])
    {
      std::cout << USAGE << std::endl;
      return (-1);
    }
  if ((file_fd = ::open(av[1], O_RDONLY, 0644)) == -1)
    {
      std::cout << EOPENFILE << std::endl;
      return (-1);
    }

  _child_pid = run_and_pause(av[1]);

  std::cout << PROMPT << std::flush;
  for (std::string tmp;
       std::getline(std::cin, tmp);
       std::cout << PROMPT << std::flush)
    {
      if (tmp.empty())
	continue ;

      unsigned long a, i = tmp.find(" ");
      std::string cmd = tmp.substr(0, i == std::string::npos ? tmp.size() : i);

      for (a = 0;a < CMD_NB;++a)
	if (!cmd.compare(cmd_tab[a]))
	  break ;

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

  std::cout << std::endl;
  (void)::kill(_child_pid, SIGTERM);
  return (::close(file_fd));
}
