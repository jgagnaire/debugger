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

class GueulDeBoua {
    private:
        std::string             _arg;
        //std::map                _breakpoint_map;
        uint64_t                _data;
        struct user_regs_struct _regs;
        int                     _child_pid;

    public:
        GueulDeBoua(std::string const& arg);
        ~GueulDeBoua();

    public:
        void        ptrace();
        void        frok();
        void        debugger(int status);
        void        break_(int);
        int        restoreBreak(int);
};

