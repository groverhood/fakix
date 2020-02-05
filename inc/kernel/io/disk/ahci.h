#ifndef FAKIX_IO_DISK_AHCI_H
#define FAKIX_IO_DISK_AHCI_H 1

#include <hd.h>
#include <stddef.h>
#include <io/dev.h>


void init_ahci_driver(struct fakix_device *dev);

size_t ahci_disk_read(void *dest, lbaddr_t src, size_t bytes);
size_t ahci_disk_write(lbaddr_t dest, const void *src, size_t bytes);

#endif