#include <fakix/tcb.h>
#include <fakix/init/start.h>

static uint8_t crt0_temp_stack[VSPACE_BASE_PAGE_SIZE * 4];

static void _start_generic(tcb_handle_t tcb, bool init)
{
    asm volatile(
        "movq sp, %[CRT0_TEMP_STACK]" 
        :: [CRT0_TEMP_STACK] "r" (crt0_temp_stack + sizeof crt0_temp_stack)
    );

    task_init_disabled(tcb, init);
}

void _start(tcb_handle_t tcb)
{
    _start_generic(tcb, false);
}

void _start_init(tcb_handle_t tcb)
{
    _start_generic(tcb, true);
}