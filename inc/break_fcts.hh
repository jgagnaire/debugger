#pragma once

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ptrace.h>
# include <sys/user.h>
# include <errno.h>
# include <string.h>

# include <string>
# include <iostream>
# include <map>
# include <iomanip>

//std::string                     _arg;
std::map<uint64_t, uint64_t>    _breakpoint_map;
uint64_t                        _data;
struct user_regs_struct         _regs;

void        set_breakpoint(uint64_t addr);
