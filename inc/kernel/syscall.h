/**
 *  === syscall.h ===
 * 
 *  This header exposes functions for interfacing with the kernel's syscall
 *  handling state, such as intialization, mapping, and allocation of syscall
 *  handlers.
 **/

#ifndef FAKIX_SYSCALL_H
#define FAKIX_SYSCALL_H 1

#include <sys.h>

/* syscaddr_t conv_kern_to_sys(FUNC *fptr); */
#define conv_kern_to_sys(fptr) ((syscaddr_t)(fptr))

void syscall_init(void);
void syscall_map(sysnum_t sysn, const struct syscall_info *info);
void syscall_add(const struct syscall_info *info);

#endif