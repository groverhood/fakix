#include <cpu/msr.h>
#include <elf/elf.h>
#include <cap/caps.h>
#include <sys/core.h>
#include <log/print.h>
#include <sys/syscall.h>
#include <init/startup.h>
#include <fakix/syscall.h>
#include <sys/task_manager.h>
#include <fakix/capabilities.h>
#include <fakix/init/bootinfo.h>

#define SPIN() do { asm (""); } while (1)

char print_buffer[VSPACE_BASE_PAGE_SIZE];

/* Setup the system calls required by a user process running on x64. */
static errval_t enable_syscalls(void);

void local_start(struct bootstruct *bootinfo)
{
    static struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    if (serial_init() != ERR_OK) {
        goto spin;
    }

    if (enable_syscalls() != ERR_OK) {
        KERNEL_MSG("Can't enable syscalls, spinning");
        goto spin;
    }

spin:
    SPIN();
}

static errval_t enable_syscalls(void)
{
    return wrmsr(IA32_LSTAR_MSR, (vaddr_t)&syscall_entry);
}
