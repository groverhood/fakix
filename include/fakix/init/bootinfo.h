#ifndef FAKIX_INIT_BOOTINFO_H
#define FAKIX_INIT_BOOTINFO_H 1

#include <stddef.h>
#include <fakix/init/bootinfo_arch.h>

struct bootinfo {
    struct bootinfo_arch arch;
    size_t ramcap_count;
    size_t devcap_count;
    size_t acpicap_count;
};

#endif