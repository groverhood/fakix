#ifndef POSIX_GLOB_H
#define POSIX_GLOB_H 1

#include <types/size_t.h>

typedef struct glob {
    size_t gl_pathc;
    char **gl_pathv;
    size_t gl_offs;
} glob_t;

#define GLOB_APPEND   0001
#define GLOB_DOOFFS   0002
#define GLOB_ERR      0004
#define GLOB_MARK     0010
#define GLOB_NOCHECK  0020
#define GLOB_NOESCAPE 0040
#define GLOB_NOSORT 0100

#define GLOB_ABORTED 1
#define GLOB_NOMATCH 2
#define GLOB_NOSPACE 3

int glob(const char *__restrict__ globpat, int flags,
         int (*errfn)(const char *, int), glob_t *__restrict__ glob);
void globfree(glob_t *glob);

#endif
