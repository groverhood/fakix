#ifndef FAKIX_INIT_RAMCAPS_H
#define FAKIX_INIT_RAMCAPS_H 1

#include <fakix/cspace.h>

#define CNODE_INIT_RAM 64
#define CAP_INIT_RAM_BASE CAP_ADDR(CNODE_INIT_RAM, 0)
#define CNODE_INIT_DEV (CNODE_INIT_RAM + 1)
#define CAP_INIT_DEV_BASE CAP_ADDR(CNODE_INIT_DEV, 0)
#define CNODE_INIT_ACPI (CNODE_INIT_RAM + 2)
#define CAP_INIT_ACPI_BASE CAP_ADDR(CNODE_INIT_ACPI, 0)

#endif