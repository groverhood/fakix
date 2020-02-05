#ifndef FAKIX_LIBFAK_SVR_MANAG_H
#define FAKIX_LIBFAK_SVR_MANAG_H

typedef int pid_t;

pid_t manag_create_proc(const char *execp, char *argv[]);
pid_t manag_fork(void);

#endif