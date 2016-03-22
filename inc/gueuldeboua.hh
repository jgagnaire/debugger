#pragma once

# include <libdwarf.h>
# include <elf.h>

# include <string>
# include <list>
# include <map>

# define CMD_NB 12
# define RUN_CMD 0
# define KILL_CMD 1
# define QUIT_CMD 2
# define SET_CMD 3
# define BREAK_CMD 4
# define DELETE_CMD 5
# define PRINT_CMD 6
# define PRINTREG_CMD 7
# define SETREG_CMD 8
# define HELP_CMD 9
# define NEXT_CMD 10
# define PRINTADDR_CMD 11

# define PROMPT "[GueulDeBoua] "
# define EOPENFILE "impossible d'ouvrir le fichier"
# define USAGE "Usage: ./GueulDeBoua programme_a_deboguer"
# define CNF "commande inexistante, tape help si t'es perdu"
# define HELP "commandes possibles: run, kill, quit, set, break, delete, print, printreg, setreg, backtrace, help"

struct var_struct
{
    long rbp_offset;
    long fbreg_offset;
};

struct func_struct
{
    Dwarf_Addr func_addr;
    uint64_t func_frame_base;
    Dwarf_Unsigned func_frame_base_bytes_nb;
    std::map<std::string, var_struct *> params_map;
    std::map<std::string, var_struct *> variables_map;
};

Elf64_Word get_globalsym_addr(std::string const &, int *);
func_struct *get_func_struct(std::string const &);

void run_fct(std::string const&);
void set_fct(std::string const&);
void break_fct(std::string const&);
void delete_fct(std::string const&);
void print_fct(std::string const&);
void printreg_fct(std::string const&);
void setreg_fct(std::string const&);
void next_fct(std::string const &); 
void printaddr_fct(std::string const&);
