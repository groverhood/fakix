#ifndef FAKIX_IO_RISKY_H
#define FAKIX_IO_RISKY_H 1

#include <portio.h>
#include <string.h>

#define COM1_PORT 0x3F8

inline static void risky_write(void *buffer, size_t bytes)
{
	unsigned char *byte_buffer = buffer;
	while (bytes-- > 0) {
		out(*byte_buffer++, COM1_PORT);
	}
}

inline static void risky_puts(const char *s)
{
	risky_write((void *)s, strlen(s));
	risky_write("\n", 1);
}

#endif