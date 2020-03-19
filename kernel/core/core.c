#include <sys/core.h>

#define REAL_CORE_MAX UINT8_MAX
#define REAL_CORE_MASK 0xff

struct core_state cores[REAL_CORE_MAX + 1];

tcb_handle_t sys_task_on_core(coreid_t core)
{
    return cores[core].current_task;
}