#include "debugger.hh"

std::map<std::string, int> regs_list = {
    {"rax", 0},
    {"rbx", 1},
    {"rcx", 2},
    {"rdx", 3},
    {"rsi", 4},
    {"rdi", 5},
    {"rbp", 6},
    {"rsp", 7},
    {"r8", 8},
    {"r9", 9},
    {"r10", 10},
    {"r11", 11},
    {"r12", 12},
    {"r13", 13},
    {"r14", 14},
    {"r15", 15}
};

static struct user_regs_struct _regs;

int32_t
setreg(int32_t _child_pid, std::string const& regs, int64_t value) {
    ptrace(PTRACE_GETREGS, _child_pid, 0, &_regs); 
    switch (regs_list[regs]) {
        case 0:
            _regs.rax = value;
            break;
        case 1:
            _regs.rbx = value;
            break;
        case 2:
            _regs.rcx = value;
            break;
        case 3:
            _regs.rdx = value;
            break;
        case 4:
            _regs.rsi = value;
            break;
        case 5:
            _regs.rdi = value;
            break;
        case 6:
            _regs.rbp = value;
            break;
        case 7:
            _regs.rsp = value;
            break;
        case 8:
            _regs.r8 = value;
            break;
        case 9:
            _regs.r9 = value;
            break;
        case 10:
            _regs.r10 = value;
            break;
        case 11:
            _regs.r11 = value;
            break;
        case 12:
            _regs.r12 = value;
            break;
        case 13:
            _regs.r13 = value;
            break;
        case 14:
            _regs.r14 = value;
            break;
        case 15:
            _regs.r15 = value;
            break;
        default:
            return -1;
    }
    ptrace(PTRACE_SETREGS, _child_pid, 0, &regs);
    return 1;
}

int32_t
printreg(int32_t _child_pid, std::string const& regs) {
    ptrace(PTRACE_GETREGS, _child_pid, 0, &_regs);
    switch (regs_list[regs]) {
        case 0:
            std::cout << regs << " = " << _regs.rax;
            break;
        case 1:
            std::cout << regs << " = " << _regs.rbx;
            break;
        case 2:
            std::cout << regs << " = " << _regs.rcx;
            break;
        case 3:
            std::cout << regs << " = " << _regs.rdx;
            break;
        case 4:
            std::cout << regs << " = " << _regs.rsi;
            break;
        case 5:
            std::cout << regs << " = " << _regs.rdi;
            break;
        case 6:
            std::cout << regs << " = " << _regs.rbp;
            break;
        case 7:
            std::cout << regs << " = " << _regs.rsp;
            break;
        case 8:
            std::cout << regs << " = " << _regs.r8;
            break;
        case 9:
            std::cout << regs << " = " << _regs.r9;
            break;
        case 10:
            std::cout << regs << " = " << _regs.r10;
            break;
        case 11:
            std::cout << regs << " = " << _regs.r11;
            break;
        case 12:
            std::cout << regs << " = " << _regs.r12;
            break;
        case 13:
            std::cout << regs << " = " << _regs.r13;
            break;
        case 14:
            std::cout << regs << " = " << _regs.r14;
            break;
        case 15:
            std::cout << regs << " = " << _regs.r15;
            break;
        default:
            return -1;
    }
    std::cout << std::endl;
    return 0;
}
