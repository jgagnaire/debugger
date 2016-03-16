#ifndef GUEULDEUBOUA_H_
# define GUEULDEUBOUA_H_

# include <elf.h>

# include <string>

# define CMD_NB 11
# define RUN_CMD 0
# define KILL_CMD 1
# define QUIT_CMD 2
# define SET_CMD 3
# define BREAK_CMD 4
# define DELETE_CMD 5
# define PRINT_CMD 6
# define PRINTREG_CMD 7
# define SETREG_CMD 8
# define BACKTRACE_CMD 9
# define HELP_CMD 10

# define PROMPT "[GueulDeBoua] "
# define EOPENFILE "impossible d'ouvrir le fichier"
# define USAGE "Usage: ./GueulDeBoua programme_a_deboguer"
# define CNF "commande inexistante, tape help si t'es perdu"
# define HELP "commandes possibles: run, kill, quit, set, break, delete, print, printreg, setreg, backtrace, help"

Elf64_Word get_sym_addr(std::string const &, int *);

void run_fct(int32_t _child_pid, std::string const&);
void set_fct(int32_t _child_pid, std::string const&);
void break_fct(int32_t _child_pid, std::string const&);
void delete_fct(int32_t _child_pid, std::string const&);
void print_fct(int32_t _child_pid, std::string const&);
void printreg_fct(int32_t _child_pid, std::string const&);
void setreg_fct(int32_t _child_pid, std::string const&);
void backtrace_fct(int32_t _child_pid, std::string const&);

#endif /* !GUEULDEBOUA_H_ */
