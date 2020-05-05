#include <sys/core.h>

coreid_t sys_core_id(void)
{
    uintptr_t word;
    asm volatile ("mrs %0, mpidr_el1" : "=r" (word));
    return (coreid_t)(word & 3);
}