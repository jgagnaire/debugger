#include "GueulDeBoua.hh"

GueulDeBoua::GueulDeBoua(std::string const& arg) : _arg(arg) {}

GueulDeBoua::~GueulDeBoua() {}

void
GueulDeBoua::frok() {
    ::execl(_arg.c_str(), _arg.c_str(), NULL);
}

void
GueulDeBoua::break_(int addr) {
    uint64_t    data = 0;
    _data = ::ptrace(PTRACE_PEEKTEXT, _child_pid, addr, 0);
    uint64_t    trapped_data = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
    if (::ptrace(PTRACE_POKETEXT, _child_pid, addr, trapped_data))
        std::cerr << strerror(errno) << std::endl;
}

int
GueulDeBoua::restoreBreak(int addr) {
    if (_regs.rip == (unsigned long) addr + 1) {
        std::cout << std::hex << _regs.rip << std::endl;
        std::cout << "data: " << _data << std::endl;
        ::ptrace(PTRACE_POKETEXT, _child_pid, addr, _data);
        std::string tmp;
        std::getline(std::cin, tmp);
        _regs.rip -= 1;
        ::ptrace(PTRACE_SETREGS, _child_pid, 0, &_regs);
        return 1;
    }
    return 0;
}

void
GueulDeBoua::debugger(int status) {
        int         count = 0;
        uint32_t    addr = 0x400562;

        break_(addr);
        if (::ptrace(PTRACE_CONT, _child_pid, 0, 0)) {
            std::cerr << strerror(errno) << std::endl;
        }
        wait(&status);

        while (1) {
            if (::ptrace(PTRACE_GETREGS, _child_pid, 0, &_regs) < 0) {
                std::cerr << strerror(errno) << std::endl;
            }

            if (restoreBreak(addr)) {
            
                ::ptrace(PTRACE_SINGLESTEP, _child_pid, 0, 0);
                wait(&status);
                if (WIFEXITED(status)) {
                    break ;
                }
                // break_(addr);
            }
            if (::ptrace(PTRACE_CONT, _child_pid, 0, 0) < 0)
                std::cerr << strerror(errno) << std::endl; 

            std::cout << "rip value: " << _regs.rip << std::endl;
            ++count;
            if (WIFEXITED(status)) {
                break ;
            } else if (WIFSTOPPED(status)) {
                return ;
            }
        }

        std::cout << "this program executed "
            << count
            << " instructions."
            << std::endl; 
}

void
GueulDeBoua::ptrace() {
    int status;

    if ((_child_pid = fork()) == 0) {
        ::ptrace(PTRACE_TRACEME);
        frok();
    } else if (_child_pid > 0) {
        wait(&status);
        debugger(status);
    } else {
        std::cerr << "fork" << std::endl;
    }
}
