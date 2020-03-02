#ifndef FAKIX_LIBC_STDLIB_H
#define FAKIX_LIBC_STDLIB_H 1

#include <stddef.h>

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void free(void *ptr);

#endif