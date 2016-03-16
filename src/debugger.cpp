#include "debugger.hh"

void
exec_child(std::string const& child_name) {
    ::ptrace(PTRACE_TRACEME);
    ::execl(child_name.c_str(), child_name.c_str(), NULL);
}

int32_t
run_and_pause(std::string const& child_name) {
    int32_t     _child_pid;
    int32_t     status;

    if ((_child_pid = fork()) == 0) {
        exec_child(child_name);
    } else if (_child_pid > 0) {
        waitpid(_child_pid, &status, 0);
        return _child_pid;
    } else {
        std::cerr << "error while forking" << std::endl;
        exit(1);
    }
}
