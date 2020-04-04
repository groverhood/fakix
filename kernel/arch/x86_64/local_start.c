#include <init/startup.h>
#include <bootboot/bootboot.h>
#include <fakix/init/bootinfo.h>

void _start(void)
{
    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    start(&bi);
}