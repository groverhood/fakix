#ifndef KERNEL_LOG_PRINT_H
#define KERNEL_LOG_PRINT_H 1

#include <stddef.h>
#include <sys/core.h>

void kernel_log(const char *msg);

void kernel_snprintf(char *dest, size_t buflen, const char *format, ...);

extern char print_buffer[VSPACE_BASE_PAGE_SIZE];

#define KERNEL_MSG(format, ...) \
    do { \
        kernel_snprintf(print_buffer, VSPACE_BASE_PAGE_SIZE, "kernel.%i: " \
                        format "\n", sys_core_id(), ##__VA_ARGS__); \
        kernel_log(print_buffer); \
    } while (0)

#endif