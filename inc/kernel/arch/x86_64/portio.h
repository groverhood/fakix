#ifndef FAKIX_ARCH_X86_64_PORTIO_H
#define FAKIX_ARCH_X86_64_PORTIO_H 1

#define out(data, port) __asm__ __volatile__ ("out %0, %%dx" :: "a" (data), "d" (port))
#define in(port, pdata) __asm__ __volatile__ ("in %%dx, %0" :: "a" (data), "d" (port))

#endif