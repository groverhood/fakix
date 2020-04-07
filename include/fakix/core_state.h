#ifndef FAKIX_CORE_STATE_H
#define FAKIX_CORE_STATE_H 1

#include <stdint.h>
#include <fakix/tcb.h>

typedef uint32_t coreid_t;

#define COREID_MAX UINT32_MAX
#define COREID_MIN UINT32_MIN

struct core_state {
    coreid_t uid;
    tcb_handle_t current_task;
};

tcb_handle_t core_current_task(coreid_t core);

#endif