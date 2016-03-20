#include <sched.h>
#include "debugger.hh"


static int
exec_child(void *child_name) {
  ::ptrace(PTRACE_TRACEME);
  return (::execl((char *)child_name, (char *)child_name, NULL));
}

int32_t
run_and_pause(std::string const& child_name) {
    int32_t     _child_pid;
    int32_t     status;
    char	*stack;
    char	*stack_beginning;

    if (!(stack = (char *)::malloc(STACK_SIZE)))
      ::exit(EXIT_FAILURE);

    // because the stack grows downwards :)
    stack_beginning = stack + STACK_SIZE;

    if ((_child_pid = ::clone(exec_child, stack_beginning,
			      SIGCHLD, (void *)child_name.c_str())) != -1) {
      ::waitpid(_child_pid, &status, 0);
      return _child_pid;
    } else {
      std::cerr << "error while clone()ing" << std::endl;
      exit(1);
    }
    return _child_pid;
}
