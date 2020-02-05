#include <stdio.h>
#include <util/panic.h>

void panic(const char *msg)
{
    printf("KERNEL PANIC :: %s\n", msg);
    while (1) {
        __asm__("");
    }
}