
#include <iostream>
#include <string>

#include "gueuldeboua.hh"
#include "debugger.hh"

extern int32_t _child_pid;

void run_fct(std::string const&)
{
    run_tracee(_child_pid);
}

void break_fct(std::string const& cmd)
{
    unsigned long i;
    Elf64_Word fctaddr;
    int error;

    if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() - 1)
    {
        std::cout << "file le nom de la fonction à laquelle tu veux breaker"
            << std::endl;
        return ;
    }
    fctaddr = get_globalsym_addr(cmd.substr(i + 1), &error);
    if (error == -1)
        return ;
    std::cout << "l'adresse à laquelle on veut mettre un point de cassure est "
        << "0x" << std::hex << fctaddr << std::dec << std::endl;
    set_breakpoint(_child_pid, fctaddr);
}

void next_fct(std::string const&) {
    singlestep_tracee(_child_pid);    
}

void delete_fct(std::string const& cmd) {
    unsigned long i;
    Elf64_Word fctaddr;
    int error;

    if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() - 1)
    {
      std::cout << "file le nom de la fonction" << std::endl;
      return ;
    }
    fctaddr = get_globalsym_addr(cmd.substr(i + 1), &error);
    if (error == -1)
        return ;
    if (unset_breakpoint(_child_pid, fctaddr) == 1)
      std::cout << "Sucessfully deleted breakpoint at address 0x"
		<< std::hex << fctaddr << std::dec << std::endl;
}

void print_fct(std::string const& cmd)
{
    std::stringstream ss(cmd);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string>    cmd_vector(begin, end);
    std::vector<std::string>::iterator it(cmd_vector.begin());
    func_struct *x;
    var_struct *y;

    if (cmd_vector.size() < 3)
    {
        std::cout << "file le nom de la fonction, celui de la variable"
            << " et 'local' ou 'param' selon le type de variable"
            << " que tu veux print"
            << std::endl;
        return ;
    }

    if (!(x = get_func_struct(*(it + 1))))
        return ;

    int32_t     ret_value;
    try {
        if (!(*(it + 3)).compare("local")) {
            if (!(y = x->variables_map.at(*(it + 2))))
                throw std::exception();
            ret_value = print_localvar(_child_pid, y->rbp_offset);
        } else if (!(*(it + 3)).compare("param")) {
            y = x->params_map.at(*(it + 2));
            ret_value = print_paramvar(_child_pid); 
        } else {
            throw std::exception();
        }
    } catch (...) {
        std::cout << "variable non trouvee, verifie que le binaire "
            << "a debogguer soit compile avec -fvar-tracking !" << std::endl;
        return ;
    }

    std::cout << std::endl
        << "variable '" << *(it + 2) << "' dans la fonction '" << *(it + 1)
        << "' à l'adresse " << "0x" << std::hex << (uint64_t)x->func_addr
        << std::dec << ":" << std::endl;
    if (y != NULL) {
        std::cout << "\t=> offset avec rbp (breg6): " << y->rbp_offset << std::endl
        << "\t=> offset avec l'adresse de '" << *(it + 1)
        << "'" << " (fbreg): " << y->fbreg_offset << std::endl << std::endl;
    }
    std::cout << "and value is " << ret_value << std::endl;
}

void
printaddr_fct(std::string const& cmd) {
    uint64_t    i;

    if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() -1) {
        return ;
    }
    std::string addr(cmd.substr(i + 1));
    printaddr(_child_pid, std::stol(addr, 0, 16));
}

void printreg_fct(std::string const& cmd)
{
    uint64_t    i;

    if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() -1) {
        return ;
    }
    std::string reg(cmd.substr(i + 1));
    if (printreg(_child_pid, reg) >= 0) {
        return ;
    }
}

void setreg_fct(std::string const& cmd)
{
    std::stringstream ss(cmd);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string>    cmd_vector(begin, end);
    std::vector<std::string>::iterator it(cmd_vector.begin());
    setreg(_child_pid, *(it + 1), std::atoi((*(it + 2)).c_str()));
}
