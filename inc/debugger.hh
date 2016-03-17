#pragma once

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ptrace.h>
# include <sys/user.h>
# include <sys/reg.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <errno.h>
# include <string.h>

# include <string>
# include <iostream>
# include <map>
# include <iomanip>

void        set_breakpoint(int32_t pid, uint64_t addr);
int32_t     unset_breakpoint(int32_t pid, uint64_t addr);
int32_t     run_and_pause(std::string const& name); 
int32_t     setreg(int32_t _child_pid, std::string reg, int64_t value);
int32_t     printreg(int32_t _child_pid, std::string const& reg);

