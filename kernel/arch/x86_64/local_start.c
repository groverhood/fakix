#include <cpu/msr.h>
#include <elf/elf.h>
#include <cap/caps.h>
#include <sys/core.h>
#include <sys/syscall.h>
#include <init/startup.h>
#include <fakix/syscall.h>
#include <sys/task_manager.h>
#include <bootboot/bootboot.h>
#include <fakix/capabilities.h>
#include <fakix/init/bootinfo.h>

/* Setup the system calls required by a user process running on x64. */
static errval_t enable_syscalls(void);

extern BOOTBOOT bootboot;

void _start(void)
{
    if (sys_core_id() != 0) {
        while (1);
    }

    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    enable_syscalls();

    start(&bi);
}

errval_t task_init(Elf64_Ehdr *hdr, struct task_manager *tm, struct capability *(*alloc_fn)(void))
{
    

    tcb_handle_t handle = tm->tcb;
    struct tcb_generic_shared *tcb = tcb_get_generic_shared(handle);
    union registers *disabled_area = &tcb->disabled_save_area;

    disabled_area->named.rdi = handle;
    tcb->pc = hdr->e_entry;
    tcb->disabled = true;

    struct capability *vroot = alloc_fn();
    caps_write_cap(tm->root, CAP_PAGE_VTL1, )
}

static errval_t enable_syscalls(void)
{
    return wrmsr(IA32_LSTAR_MSR, (vaddr_t)&syscall_entry);
}