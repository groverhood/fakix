
#include <stdio.h>
#include <stdarg.h>
#include <io/risky.h>
#include <io/serial.h>
#include <paging/conv.h>

static char printfbuf[FAKIX_PAGE];

int vprintf(const char *format, va_list argv)
{
	vsprintf(printfbuf, format, argv);
	
	if (serial_initialized) {

	} else {
		risky_write(printfbuf, strlen(printfbuf));
	}
}