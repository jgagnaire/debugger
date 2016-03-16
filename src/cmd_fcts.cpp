
#include <iostream>
#include <string>

#include "gueuldeboua.hh"
#include "debugger.hh"

void run_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [run] (et lekudlashat aussi)" << std::endl;
}

void set_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [set] (et leusex aussi)" << std::endl;
}

void break_fct(int32_t _child_pid, std::string const& cmd)
{
  unsigned long i;
  unsigned long symaddr;
  int error;

  if ((i = cmd.find(" ")) == std::string::npos or i == cmd.size() - 1)
    {
      std::cout << "file le nom de la fonction à laquelle tu veux breaker"
                << std::endl;
      return ;
    }
  symaddr = get_sym_addr(cmd.substr(i + 1), &error);
  if (error == -1)
    return ;
  std::cout << "l'adresse à laquelle on veut mettre un point de cassure est "
            << "0x" << std::hex << symaddr << std::endl;
  set_breakpoint(_child_pid, symaddr);
}

void delete_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [delete] (et lekouy aussi)" << std::endl;
}

void print_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [print] (et leuzobe aussi)" << std::endl;
}

void printreg_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [printreg] (et leuphyon aussi)" << std::endl;
}

void setreg_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [setreg] (et leuku aussi)" << std::endl;
}

void backtrace_fct(int32_t _child_pid, std::string const&)
{
  std::cout << "commande: [backtrace] (et leuku aussi)" << std::endl;
}
