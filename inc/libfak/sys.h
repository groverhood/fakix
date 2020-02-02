/**
 *  === sys.h ===
 * 
 *  Userland interface for system calls.
 * 
 **/

#ifndef FAKIX_LIBFAK_SYS_H
#define FAKIX_LIBFAK_SYS_H 1

#include <stdint.h>
#include <stdbool.h>
#include <macro.h>

/* This header contains architecture-specific trap functions. */
#include <systrap.h>

typedef int sysnum_t;
typedef int sysargc_t;
typedef uintptr_t syscaddr_t;

struct syscall_info {
    sysargc_t argc; /* Argument count. */
    syscaddr_t fptr; /* Syscall address. */
};

bool sys_info(sysnum_t sysn, struct syscall_info *res_info);

/* Header for readability. */
void sys_trap(sysnum_t sysn, ...);

#define __sys_argc__(z, a, b, c, d, e, f, count, ...) count
#define sys_argc(...) macro_expand(__sys_argc__(, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0))

#define sys_trap(sysn, ...) \
    do { \
        if (sys_info(sysn, NULL)) {\
            sys_trap ## macro_expand(sys_argc(__VA_ARGS__)) ## (__VA_ARGS__); \
        } \
    } while (0)

#endif