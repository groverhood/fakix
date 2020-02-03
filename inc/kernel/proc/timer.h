/**
 *  === timer.h ===
 *  
 *  This header exposes an abstract timer interface that redirects timer
 *  interrupts to a scheduler daemon as commands.
 **/

#ifndef FAKIX_PROC_TIMER_H
#define FAKIX_PROC_TIMER_H 1

void timer_init(void);

#endif
