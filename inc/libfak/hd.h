#ifndef FAKIX_LIBFAK_HD_H
#define FAKIX_LIBFAK_HD_H 1

#include <stddef.h>
#include <stdint.h>
#include <sys.h>

typedef uintptr_t lbaddr_t;

size_t hd_write(lbaddr_t dest, const void *src, size_t bytes) kernel_unfriendly;
size_t hd_read(void *dest, lbaddr_t src, size_t bytes) kernel_unfriendly;

#define SYS_HD_WRITE SYSNUM(0x20)
#define SYS_HD_READ SYSNUM(0x21)

#endif