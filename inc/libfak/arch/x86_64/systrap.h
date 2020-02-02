#ifndef FAKIX_LIBFAK_ARCH_X86_64_SYSTRAP_H
#define FAKIX_LIBFAK_ARCH_X86_64_SYSTRAP_H 1

#define sys_trap6(sysn, arg0, arg1, arg2, arg3, arg4, arg5) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2), "c" (arg3), "8" (arg4), "9" (arg5))
#define sys_trap5(sysn, arg0, arg1, arg2, arg3, arg4) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2), "c" (arg3), "8" (arg4))
#define sys_trap4(sysn, arg0, arg1, arg2, arg3) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2), "c" (arg3))
#define sys_trap3(sysn, arg0, arg1, arg2) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn), "D" (arg0), "S" (arg1) \
        "d" (arg2))
#define sys_trap2(sysn, arg0, arg1) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn), "D" (arg0), "S" (arg1))
#define sys_trap1(sysn, arg0) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn), "D" (arg0))
#define sys_trap0(sysn) \
    __asm__ __volatile__ ("syscall" :: "a" (sysn))

#endif