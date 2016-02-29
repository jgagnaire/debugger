#include "GueulDeBoua.hh"

GueulDeBoua::GueulDeBoua(std::string const& arg) : _arg(arg) {}

GueulDeBoua::~GueulDeBoua() {}

void
GueulDeBoua::frok() {
    ::execl(_arg.c_str(), _arg.c_str(), NULL);
}

void
GueulDeBoua::debugger(int & status) {
        int count = 0;

        while (WIFSTOPPED(status)) {
            if (::ptrace(PTRACE_SINGLESTEP, _child_pid, 0, 0) < 0) {
                std::cerr << "ptrace" << std::endl;
            }
            ++count;
            wait(&status);
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
        ::ptrace(PTRACE_TRACEME, 0, 0, 0);
        frok();
    } else if (_child_pid > 0) {
        wait(&status);
        debugger(status);
    } else {
        std::cerr << "fork" << std::endl;
    }
}
