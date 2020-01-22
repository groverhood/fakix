#ifndef FAKIX_IO_DISK_H
#define FAKIX_IO_DISK_H 1

#include <stddef.h>
#include <sys/types.h>

void init_disk_driver(void);

typedef ssize_t disk_read_op_t(void *dest, off_t start, size_t bytes);
typedef ssize_t disk_write_op_t(const void *dest, off_t start, size_t bytes);

ssize_t disk_read(void *dest, off_t start, size_t bytes);
ssize_t disk_write(const void *src, off_t start, size_t bytes);

#endif