#include <msr.h>
#include <cap/caps.h>
#include <init/startup.h>
#include <fakix/syscall.h>
#include <bootboot/bootboot.h>
#include <fakix/capabilities.h>
#include <fakix/init/bootinfo.h>

static errval_t syscall_default(void)
{
    return ERR_OK;
}

void *syscall_table[SYS_COUNT] = {
    [0 ... SYS_COUNT - 1] = &syscall_default,
    [SYS_INVOKE_CAP] = &cap_invoke,
    [SYS_CREATE_CAP] = &caps_create,
    [SYS_RETYPE_CAP] = &caps_retype,
    [SYS_DESTROY_CAP] = &caps_destroy,
    [SYS_COPY_CAP] = &caps_copy,
    [SYS_MAP_CAP] = &caps_vmap
};

/* Setup the system calls required by a user process running on x64. */
static void enable_syscalls(void);

void _start(void)
{
    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    enable_syscalls();

    start(&bi);
}

static void enable_syscalls(void)
{
    
}