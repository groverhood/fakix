#include <dev/apic.h>
#include <sys/core.h>
#include <msr.h>

coreid_t sys_core_id(void)
{
    return *LAPIC_ID_REGISTER;
}