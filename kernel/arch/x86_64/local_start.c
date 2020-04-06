#include <cpu/msr.h>
#include <cap/caps.h>
#include <sys/syscall.h>
#include <init/startup.h>
#include <fakix/syscall.h>
#include <bootboot/bootboot.h>
#include <fakix/capabilities.h>
#include <fakix/init/bootinfo.h>

/* Setup the system calls required by a user process running on x64. */
static errval_t enable_syscalls(void);

void _start(void)
{
    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    enable_syscalls();

    start(&bi);
}

static errval_t enable_syscalls(void)
{
    return wrmsr(IA32_LSTAR_MSR, (vaddr_t)&syscall_entry);
}