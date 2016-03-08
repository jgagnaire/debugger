#include "DebugInformation.hh"

DebugInformation::DebugInformation(std::string const& progname) {
    int     fd;

    if ((fd = open(progname.c_str(), O_RDONLY)) > 0) {
        std::cerr << "open" << std::endl;
    }
    dwarf_init(fd, DW_DLC_READ, 0, 0, &_dbg, &_err);
}

DebugInformation::~DebugInformation() {}

void
DebugInformation::list_cu() {
	Dwarf_Unsigned cu_header_length, abbrev_offset, next_cu_header;
    Dwarf_Half version_stamp, address_size;
    Dwarf_Error err;
    Dwarf_Die no_die = 0, cu_die, child_die;

    /* find cu */
    if (dwarf_next_cu_header(
                _dbg,
                &cu_header_length,
                &version_stamp,
                &abbrev_offset,
                &address_size,
                &next_cu_header,
                &err) == DW_DLV_ERROR) {
        std::cerr << "le cul n'est pas présent" << std::endl;
    }

    std::cout << "cu_header_lenght: " << cu_header_length
        << std::endl;
    if (dwarf_siblingof(_dbg, no_die, &cu_die, &err) == DW_DLV_ERROR) {
        std::cerr << "le cul n'a pas de soeur" << std::endl;
    }

    if (dwarf_child(cu_die, &child_die, &err) == DW_DLV_ERROR) {
        std::cerr << "la soeur du cul n'a pas de fils" << std::endl;
    }

    while (1) {
        int res;
    
        list_cu_func(child_die);

        res = dwarf_siblingof(_dbg, child_die, &child_die, &err);

        if (res == DW_DLV_ERROR)
            std::cerr << "le fils de la soeur du cul n'a pas de soeur" 
                << std::endl;
        else if (res == DW_DLV_NO_ENTRY) {
            std::cout << "c'est la fin de l'histoire d'un cul" << std::endl;
            break;
        }
    }
}

void
DebugInformation::list_cu_func(Dwarf_Die const& child_die) {
    Dwarf_Half      tag;
    char const *    tag_name;

    dwarf_tag(child_die, &tag, &_err);
    /* not interested in no function */
    if (tag != DW_TAG_subprogram) {
        return;
    }
    dwarf_get_TAG_name(tag, &tag_name);
    char * name;
    dwarf_diename(child_die, &name, &_err);
    std::cout << "the name of the die is: " << name << std::endl;
}
