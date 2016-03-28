#pragma once

# include <libdwarf.h>
# include <elf.h>

# include <string>
# include <list>
# include <map>

# define CMD_NB 10
# define RUN_CMD 0
# define QUIT_CMD 1
# define BREAK_CMD 2
# define DELETE_CMD 3
# define PRINT_CMD 4
# define PRINTREG_CMD 5
# define SETREG_CMD 6
# define HELP_CMD 7
# define NEXT_CMD 8
# define PRINTADDR_CMD 9

# define PROMPT "[my_gdb] "
# define EOPENFILE "impossible d'ouvrir le fichier"
# define USAGE "Usage: ./my_gdb program_to_debug"
# define CNF "command not found, type 'help' if needed"
# define HELP "implemented commands: run, quit, break, delete, print, printreg, setreg, help"

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
