#ifndef KERNEL_SYS_SYSCALL_H
#define KERNEL_SYS_SYSCALL_H 1

#include <fakix/errtype.h>
#include <fakix/syscall.h>

errval_t syscall_entry(sysarg_t arg0, sysarg_t arg1, sysarg_t arg2, 
                       sysarg_t arg3, sysarg_t arg4, sysarg_t arg5);

#endif