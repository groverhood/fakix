#include <sys/core.h>
#include <log/print.h>
#include <bootboot/bootboot.h>
#include <io/mmio_rpi4b_gpio.h>
#include <fakix/init/bootinfo.h>

extern BOOTBOOT bootboot;

void _start(void)
{
    if (sys_core_id() != bootboot.bspid) {
        asm("");
    }

    struct bootinfo bi;
    struct bootinfo_arch *arch = &bi.arch;

    vaddr_t mmio = bootboot.arch.aarch64.mmio_ptr;
    volatile struct mmio_rpi4b_gpio *gpio = 
        (struct mmio_rpi4b_gpio *)(MMIO_RPI4B_GPIO_BASE + mmio);

    gpio->gpfsel4.fsel7 = 001;
    gpio->gplev.levn &= ~(1UL << 47);

    while (1) asm("");
}