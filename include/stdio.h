#ifndef FAKIX_LIBC_STDIO_H
#define FAKIX_LIBC_STDIO_H

#include <stddef.h>
#include <stdarg.h>
#include <attributes.h>

/* Discrete marker with all bits set. */
#define EOF (-1)

/* Opaque I/O stream object. Use this over file descriptors whenever 
   applicable. This struct is not available in kernel mode. */
typedef struct _fdbuf FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

/* These functions are all available in kernel mode. */

int puts(const char *s);
int vsnprintf(char *dest, size_t sz, const char *format, va_list argv);
int vsprintf(char *dest, const char *format, va_list argv);
int snprintf(char *dest, size_t sz, const char *format, ...);
int sprintf(char *dest, const char *format, ...);
int vprintf(const char *format, va_list argv);
int printf(const char *format, ...);
int putchar(int c);
int getchar(void);

/* END kernel-friendly functions. */

FILE *fopen(const char *path, const char *modes) kernel_unfriendly;
FILE *fdopen(int fd, const char *modes) kernel_unfriendly;
void fclose(FILE *stream) kernel_unfriendly;
void fflush(FILE *stream) kernel_unfriendly;
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) kernel_unfriendly;
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) kernel_unfriendly;
int feof(FILE *stream) kernel_unfriendly;
int ferror(FILE *stream) kernel_unfriendly;
int fileno(FILE *stream) kernel_unfriendly;
int vfprintf(FILE *stream, const char *format, va_list argv) kernel_unfriendly;
int fprintf(FILE *stream, const char *format, ...) kernel_unfriendly;
int vdprintf(int fd, const char *format, va_list argv) kernel_unfriendly;
int dprintf(int fd, const char *format, ...) kernel_unfriendly;
int fputc(int c, FILE *stream) kernel_unfriendly;
int vfscanf(FILE *stream, const char *format, va_list argv) kernel_unfriendly;
int fscanf(FILE *stream, const char *format, ...) kernel_unfriendly;

void flockfile(FILE *stream) kernel_unfriendly;
void funlockfile(FILE *stream) kernel_unfriendly;

#define putc fputc

int vsscanf(char *str, const char *format, va_list argv) kernel_unfriendly;
int sscanf(char *str, const char *format, ...) kernel_unfriendly;
int vscanf(const char *format, va_list argv) kernel_unfriendly;
int scanf(const char *format, ...) kernel_unfriendly;

#endif