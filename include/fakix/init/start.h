#ifndef FAKIX_INIT_START_H
#define FAKIX_INIT_START_H 1

#include <fakix/tcb.h>
#include <attributes.h>

__BEGIN_DECLS

void task_init_disabled(tcb_handle_t tcb, bool init);

__END_DECLS

#endif