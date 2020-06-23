#ifndef POSIX_DLFCN_H
#define POSIX_DLFCN_H 1

#define RTLD_LAZY 000
#define RTLD_NOW  001
#define RTLD_GLOBAL 002
#define RTLD_LOCAL 000

int dlclose(void *);
char *dlerror(void);
void *dlopen(const char *, int);
void *dlsym(void *__restrict__, const char *__restrict__);

#endif
