#ifndef KERNEL_SYS_CORE_H
#define KERNEL_SYS_CORE_H 1

#include <stdint.h>
#include <fakix/tcb.h>

typedef uint32_t coreid_t;

#define COREID_MAX UINT32_MAX
#define COREID_MIN UINT32_MIN

struct core_state {
    coreid_t uid;
    tcb_handle_t current_task;
};

coreid_t sys_core_id(void);
tcb_handle_t sys_task_on_core(coreid_t core);

#endif