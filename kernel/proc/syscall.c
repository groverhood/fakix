#include <attributes.h>
#include <cmd.h>
#include <switch.h>
#include <syscall.h>
#include <task.h>

syscaddr_t syscall_table[SYSNUM_MAX + 1];

void syscall_map(sysnum_t sysn, syscaddr_t fptr)
{
    if (sysn >= 0 && sysn <= SYSNUM_MAX) {
        syscall_table[sysn] = fptr;
    }
}

void syscall_add(syscaddr_t fptr)
{
    sysnum_t sysn;
    for (sysn = 0; sysn <= SYSNUM_MAX; ++sysn) {
        if (syscall_table[sysn] == 0) {
            syscall_table[sysn] = fptr;
            break;
        }
    }
}