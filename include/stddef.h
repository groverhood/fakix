#ifndef FAKIX_LIBC_STDDEF_H
#define FAKIX_LIBC_STDDEF_H 1

#include <stdint.h>

typedef uint64_t size_t;
typedef int64_t ssize_t;

typedef ssize_t ptrdiff_t;

typedef int32_t wchar_t;

#define NULL ((void *)0)
#define offsetof(type, member) \
    ((size_t)((unsigned char *)(&((type *)NULL)->member) - ((unsigned char *)NULL)))

#endif