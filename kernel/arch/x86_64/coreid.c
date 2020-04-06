#include <cpu/msr.h>
#include <dev/apic.h>
#include <sys/core.h>

coreid_t sys_core_id(void)
{
    return *LAPIC_ID_REGISTER;
}