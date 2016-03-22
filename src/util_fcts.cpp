#include <cstring>

#include "debugger.hh"

void
run_tracee(int32_t _child_pid) {
    std::cout << "running child" << std::endl; 
    if (ptrace(PTRACE_CONT, _child_pid, 0, 0) < 0)
        std::cerr << "error trying to run the tracee" << std::endl;
}

void
singlestep_tracee(int32_t _child_pid) {
    static struct user_regs_struct  _regs;
    std::cout << "sending singlestep instruction" << std::endl;
    if (ptrace(PTRACE_SINGLESTEP, _child_pid, 0, 0) < 0)
        std::cerr << "error trying to run the tracee" << std::endl;
    ::ptrace(PTRACE_GETREGS, _child_pid, 0, &_regs);
    std::cout << "0x" << std::hex << _regs.rip << std::dec << std::endl; 
}

void
printaddr(uint32_t _child_pid, uint64_t addr) {
    int32_t res;
    res = ::ptrace(PTRACE_PEEKTEXT, _child_pid, addr, 0);
    std::cout << "res is " << res << std::endl;
}
