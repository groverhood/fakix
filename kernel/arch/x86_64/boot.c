
#include <stddef.h>
#include <bootboot/bootboot.h>

extern BOOTBOOT bootboot;

void _start(void)
{
    volatile char *foo = NULL;
    int c = *foo;
}