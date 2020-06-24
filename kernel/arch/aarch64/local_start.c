#include <sys/core.h>
#include <log/print.h>
#include <init/startup.h>
#include <fakix/init/bootinfo.h>

char print_buffer[VSPACE_BASE_PAGE_SIZE];

#define SPIN() do { asm (""); } while (1)

void local_start(struct bootstruct *boot)
{
    KERNEL_MSG("Hello, world!\n");
    SPIN();
}
