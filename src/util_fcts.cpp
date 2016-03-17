#include "debugger.hh"

void
run_tracee(int32_t _child_pid) {
    ptrace(PTRACE_CONT, _child_pid, 0, 0);
}
