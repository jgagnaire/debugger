#pragma once

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dwarf.h>
# include <libdwarf.h>

# include <string>
# include <iostream>

class DebugInformation {
    private:
        Dwarf_Debug     _dbg;
        Dwarf_Error      _err;

    public:
        DebugInformation(std::string const&);
        ~DebugInformation();

    public:
        void        list_cu();
        void        list_cu_func(Dwarf_Die const& child_die);
};
