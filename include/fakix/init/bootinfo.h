#ifndef FAKIX_INIT_BOOTINFO_H
#define FAKIX_INIT_BOOTINFO_H 1

#include <fakix/init/bootinfo_arch.h>

struct bootinfo {
    struct bootinfo_arch arch;
};

#endif