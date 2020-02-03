#ifndef FAKIX_LIBFAK_SVR_SCHED_H
#define FAKIX_LIBFAK_SVR_SCHED_H 1

#include <stdbool.h>
#include <sys/types.h>

pid_t sched_create(const char *execp, char *argv[]);
pid_t sched_clone(void);
int sched_kill(pid_t proc, bool block);
void sched_next(void);

#endif