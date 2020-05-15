#include <sys/core.h>
#include <log/print.h>
#include <io/mmio_rpi4b_gpio.h>
#include <init/startup.h>
#include <fakix/init/bootinfo.h>
#include <io/mmio.h>

void _start(struct bootstruct *boot)
{
    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;
    
    vaddr_t mmio = MMIO_RPI4B_LEGACY_PERIPHERAL_BASE;
    volatile struct mmio_rpi4b_gpio *gpio = 
        (struct mmio_rpi4b_gpio *)(MMIO_RPI4B_GPIO_BASE + mmio);

    gpio->gpfsel4.fsel7 = 001;
    gpio->gplev.levn &= ~(1UL << 47);
    while (1) asm("");
}
