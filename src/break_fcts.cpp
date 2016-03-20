#include "debugger.hh"

std::map<uint64_t, uint64_t>    _breakpoint_map;
uint64_t                        _data;
static struct user_regs_struct         _regs;

void
set_breakpoint(int32_t _child_pid, uint64_t addr) {
    uint64_t    data = 0;
    _data = ::ptrace(PTRACE_PEEKTEXT, _child_pid, addr, 0);
    _breakpoint_map[addr] = _data;
    uint64_t    trapped_data = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
    if (::ptrace(PTRACE_POKETEXT, _child_pid, addr, trapped_data) == -1)
      {
	std::cerr << ::strerror(errno) << std::endl;
	::exit(EXIT_FAILURE);
      }
}

int32_t
unset_breakpoint(int32_t _child_pid, uint64_t addr) {
    uint64_t    to_restore;
    // try to find the addr in the breakpoint map
    if (_breakpoint_map.find(addr) == _breakpoint_map.end()) {
        return 2;
    } else {
        to_restore = _breakpoint_map[addr];
    }
    // check regs to see if you're on the right step
    ::ptrace(PTRACE_GETREGS, _child_pid, 0, &_regs);
    if (_regs.rip == (unsigned long) addr + 1) {
        ::ptrace(PTRACE_POKETEXT, _child_pid, addr, to_restore);
        _regs.rip -= 1;
        ::ptrace(PTRACE_SETREGS, _child_pid, 0, &_regs);
        return 1;
    }
    return 0;
}
