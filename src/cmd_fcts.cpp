
#include <iostream>
#include <string>

#include "gueuldeboua.hh"
#include "debugger.hh"

extern int32_t _child_pid;

void run_fct(std::string const&)
{
  std::cout << "commande: [run] (et lekudlashat aussi)" << std::endl;
}

void set_fct(std::string const&)
{
  std::cout << "commande: [set] (et leusex aussi)" << std::endl;
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
            << "0x" << std::hex << fctaddr << std::endl;
  set_breakpoint(_child_pid, fctaddr);
}

void delete_fct(std::string const&)
{
  std::cout << "commande: [delete] (et lekouy aussi)" << std::endl;
}

void print_fct(std::string const& cmd)
{
  unsigned long i, varsize;
  Elf64_Word varaddr;

  if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() - 1)
    {
      std::cout << "file le nom de la variable que tu veux print"
                << std::endl;
      return ;
    }
  if (get_localvar_addr(cmd.substr(i + 1), &varaddr, &varsize) == -1)
    return ;
  //print_variable_value(varaddr, varsize);
}

void printreg_fct(int32_t _child_pid, std::string const& cmd)
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

void setreg_fct(int32_t _child_pid, std::string const& cmd)
{
  std::cout << "commande: [setreg] (et leuku aussi)" << std::endl;
}

void backtrace_fct(std::string const&)
{
  std::cout << "commande: [backtrace] (et leuku aussi)" << std::endl;
}
