#include <cmd.h>
#include <fakix.h>
#include <hd.h>
#include <io/dev.h>
#include <io/disk.h>
#include <io/serial.h>
#include <paging/alloc.h>
#include <paging/table.h>
#include <stdio.h>
#include <syscall.h>
#include <proc/task.h>

static void map_syscalls(void);
static void launch_managsvr(void);

int fakix_main(struct fakix_system_table *systable)
{
    init_devices(systable->rsdt);
    init_serial();
    init_page_alloc(systable->mmap);

    map_syscalls();

    launch_managsvr();

    return 0;
}

static void map_syscalls(void)
{
    syscall_map(SYS_CMD_SEND, conv_kern_to_sys(&cmd_send));
    syscall_map(SYS_CMD_RECV, conv_kern_to_sys(&cmd_recv));
    syscall_map(SYS_HD_WRITE, conv_kern_to_sys(&disk_write));
    syscall_map(SYS_HD_READ, conv_kern_to_sys(&disk_read));
}

static struct task managsvr;

static void launch_managsvr(void)
{

}