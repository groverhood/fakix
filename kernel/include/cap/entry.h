#ifndef KERNEL_CAPS_ENTRY_H
#define KERNEL_CAPS_ENTRY_H 1

#include <fakix/capabilities.h>

/* Entry point for SYS_CREATE_CAP. */
errval_t caps_create_entry(void *table, size_t buflen, enum cap_object_type objtype,
                           caprights_t rights, capaddr_t dest);

#endif