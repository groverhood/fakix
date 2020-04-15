
#include <log/print.h>
#include <bootboot/bootboot.h>
#include <fakix/init/bootinfo.h>


extern BOOTBOOT bootboot;

void _start(void)
{
    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    kernel_log("Hello, world!\n");

    while (1) asm("");
}