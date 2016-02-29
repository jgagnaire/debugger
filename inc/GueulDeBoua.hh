#pragma once

# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/ptrace.h>


# include <string>
# include <iostream>

class GueulDeBoua {
    private:
        std::string     _arg;
        int             _child_pid;

    public:
        GueulDeBoua(std::string const& arg);
        ~GueulDeBoua();

    public:
        void        ptrace();
        void        frok();
        void        debugger(int & status);
};
