#ifndef FAKIX_IO_DISK_AHCI_H
#define FAKIX_IO_DISK_AHCI_H 1

#include <stddef.h>
#include <io/dev.h>
#include <sys/types.h>

void init_ahci_driver(struct fakix_device *dev);

ssize_t ahci_disk_read(void *dest, off_t start, size_t bytes);
ssize_t ahci_disk_write(const void *src, off_t start, size_t bytes);

#endif