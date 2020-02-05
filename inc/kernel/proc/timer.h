/**
 *  === timer.h ===
 *  
 *  This header exposes an abstract timer interface that redirects timer
 *  interrupts to a scheduler daemon as commands.
 **/

#ifndef FAKIX_PROC_TIMER_H
#define FAKIX_PROC_TIMER_H 1

#include <io/interrupt.h>

void timer_init(void);

extern interrupt_handler_t timer_handler;

#endif
