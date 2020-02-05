#ifndef FAKIX_IO_INT_INTERRUPT_H
#define FAKIX_IO_INT_INTERRUPT_H 1

/* Architecture-specific interrupt state. */
#include <interrupt_state.h>
/* Architecture-specific interrupt codes. */
#include <interrupt_code.h>

typedef void interrupt_handler_t(struct interrupt_state *intr_state);

void interrupt_init(void);
void interrupt_set_handler(intrcode_t intr, interrupt_handler_t *handler);

#endif