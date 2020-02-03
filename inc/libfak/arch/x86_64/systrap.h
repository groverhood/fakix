#ifndef FAKIX_LIBFAK_ARCH_X86_64_SYSTRAP_H
#define FAKIX_LIBFAK_ARCH_X86_64_SYSTRAP_H 1

#define sys_trap6(sysn, rval, arg0, arg1, arg2, arg3, arg4, arg5) \
    __asm__ __volatile__ ( \
        "movq %[arg3], %%r8\n\t" \
        "movq %[arg4], %%r9\n\t" \
        "movq %[arg5], %%r10\n\t" \
        "syscall" : "=a" (rval) : "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2), [arg3] "r" (arg3), [arg4] "r" (arg4), [arg5] "r" (arg5) \
        : "r8", "r9", "r10")
#define sys_trap5(sysn, rval, arg0, arg1, arg2, arg3, arg4) \
    __asm__ __volatile__ ( \
        "movq %[arg3], %%r8\n\t" \
        "movq %[arg4], %%r9\n\t" \
        "syscall" : "=a" (rval) : "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2), [arg3] "r" (arg3), [arg4] "r" (arg4) \
        : "r8", "r9")
#define sys_trap4(sysn, rval, arg0, arg1, arg2, arg3) \
    __asm__ __volatile__ ( \
        "movq %[arg3], %%r8\n\t" \
        "syscall" : "=a" (rval) : "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2), [arg3] "r" (arg3) \
        : "r8")
#define sys_trap3(sysn, rval, arg0, arg1, arg2) \
    __asm__ __volatile__ ("syscall" : "=a" (rval) : "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2))
#define sys_trap2(sysn, rval, arg0, arg1) \
    __asm__ __volatile__ ("syscall" : "=a" (rval) : "a" (sysn), "D" (arg0), "S" (arg1))
#define sys_trap1(sysn, rval, arg0) \
    __asm__ __volatile__ ("syscall" : "=a" (rval) : "a" (sysn), "D" (arg0))
#define sys_trap0(sysn, rval) \
    __asm__ __volatile__ ("syscall" : "=a" (rval) : "a" (sysn))

#endif