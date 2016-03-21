
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
        "help", "next"};

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
    for (std::string tmp;;)
    {
        struct timeval tv;
        fd_set set;
        int32_t rv;
        int32_t status;

        FD_ZERO(&set);
        FD_SET(0, &set);

        tv.tv_sec = 0;
        tv.tv_usec = 700;

        rv = ::select(1, &set, 0, 0, &tv);
        if (rv == -1) {
            break ;
        } else if (rv == 0) { 
            waitpid(_child_pid, &status, WNOHANG);
            if (WIFSTOPPED(status) && WSTOPSIG(status) == 5) {
                check_for_breakpoint(_child_pid);
            }
        } else {
            if (not std::getline(std::cin, tmp)) {
                break ;
            }
        }
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
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case QUIT_CMD:
		  goto exit_label;
                case KILL_CMD:
		  set_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case BREAK_CMD:
                    break_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case DELETE_CMD:
                    delete_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case PRINT_CMD:
                    print_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case PRINTREG_CMD:
                    printreg_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case SETREG_CMD:
                    setreg_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case BACKTRACE_CMD:
                    backtrace_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case HELP_CMD:
                    std::cout << HELP << std::endl;
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                case NEXT_CMD:
                    next_fct(tmp);
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
                default:
                    std::cerr << CNF << std::endl;
                    std::cout << PROMPT << std::flush;
                    tmp.clear();
                    break ;
            }
        }

 exit_label:
    std::cout << std::endl;
    (void)::kill(_child_pid, SIGTERM);
    return (::close(file_fd));
}
