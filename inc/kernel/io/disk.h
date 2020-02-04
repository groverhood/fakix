#ifndef FAKIX_IO_DISK_H
#define FAKIX_IO_DISK_H 1

#include <hd.h>
#include <stddef.h>
#include <sys/types.h>

void init_disk_driver(void);

typedef size_t disk_read_op_t(void *dest, lbaddr_t src, size_t bytes);
typedef size_t disk_write_op_t(lbaddr_t dest, const void *src, size_t bytes);

size_t disk_read(void *dest, lbaddr_t src, size_t bytes);
size_t disk_write(lbaddr_t dest, const void *src, size_t bytes);

#endif