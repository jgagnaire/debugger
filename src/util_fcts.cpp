#include "debugger.hh"

void
run_tracee(int32_t _child_pid) {
    std::cout << "running child" << std::endl; 
    if (ptrace(PTRACE_CONT, _child_pid, 0, 0) < 0)
        std::cerr << "error trying to run the tracee" << std::endl;
}

void
singlestep_tracee(int32_t _child_pid) {
    std::cout << "sending singlestep instruction" << std::endl;
    if (ptrace(PTRACE_SINGLESTEP, _child_pid, 0, 0) < 0)
        std::cerr << "error trying to run the tracee" << std::endl;
}
