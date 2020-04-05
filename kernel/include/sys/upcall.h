
#ifndef KERNEL_SYS_UPCALL_H
#define KERNEL_SYS_UPCALL_H 1

#include <fakix/errtype.h>
#include <cpu/registers.h>
#include <fakix/addr.h>
#include <fakix/tcb.h>

typedef vaddr_t upcall_addr_t;

/* Perform an upcall into the provided user address. */
errval_t upcall_jump(upcall_addr_t uaddr);

#endif