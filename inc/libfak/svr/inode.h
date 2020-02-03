#ifndef FAKIX_LIBFAK_SVR_INODE_H
#define FAKIX_LIBFAK_SVR_INODE_H 1

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef intptr_t inode_t;

struct inode_capabilities {

};

inode_t inode_create(const char *path, struct inode_capabilities cap);
bool inode_remove(inode_t inode);

ssize_t inode_write(inode_t dest, const void *src, size_t bytes);
ssize_t inode_read(void *dest, inode_t src, size_t bytes);

#endif