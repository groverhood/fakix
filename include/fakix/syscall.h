#ifndef FAKIX_SYSCALL_H
#define FAKIX_SYSCALL_H 1

#ifndef ASM_FILE

#include <fakix/syscall_arch.h>

#define syscall0(sysn) syscall_arch(0, 0, 0, 0, 0, 0, sysn)
#define syscall1(sysn, arg0) syscall_arch(arg0, 0, 0, 0, 0, 0, sysn)
#define syscall2(sysn, arg0, arg1) syscall_arch(arg0, arg1, 0, 0, 0, 0, sysn)
#define syscall3(sysn, arg0, arg1, arg2) \
    syscall_arch(arg0, arg1, arg2, 0, 0, 0, sysn)
#define syscall4(sysn, arg0, arg1, arg2, arg3) \
    syscall_arch(arg0, arg1, arg2, arg3, 0, 0, sysn)
#define syscall5(sysn, arg0, arg1, arg2, arg3, arg4) \
    syscall_arch(arg0, arg1, arg2, arg3, arg4, 0, sysn)
#define syscall5(sysn, arg0, arg1, arg2, arg3, arg4, arg5) \
    syscall_arch(arg0, arg1, arg2, arg3, arg4, arg5, sysn)

typedef sysarg_t sysnum_t;

extern void **syscall_table;

#endif

#define SYS_INVOKE_CAP  0
#define SYS_CREATE_CAP  1
#define SYS_RETYPE_CAP  2
#define SYS_DESTROY_CAP 3
#define SYS_COPY_CAP    4
#define SYS_MAP_CAP     5

/* Max number of system calls. */
#define SYS_COUNT 16

#endif