#include <fakix.h>
#include <io/dev.h>
#include <io/serial.h>
#include <paging/alloc.h>
#include <paging/table.h>
#include <syscall.h>
#include <stdio.h>
#include <task.h>
#include <cmd.h>

static void map_syscalls(void);

int fakix_main(struct fakix_system_table *systable)
{
    init_devices(systable->rsdt);
    init_serial();
    init_page_alloc(systable->mmap);

    map_syscalls();

    return 0;
}

static void map_syscalls(void)
{
    syscall_map(SYS_CMD_SEND, conv_kern_to_sys(&cmd_send));
    syscall_map(SYS_CMD_RECV, conv_kern_to_sys(&cmd_recv));
    syscall_map(SYS_TASK_SCHED_NEXT, conv_kern_to_sys(&task_switch_to));
}