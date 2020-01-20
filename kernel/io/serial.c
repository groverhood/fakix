#include <io/dev.h>
#include <io/serial.h>
#include <string.h>
#include <stdio.h>

#define DEV_SERIAL 0x00

static struct fakix_device *sdev;
bool serial_initialized = false;

void init_serial(void)
{
	puts("Attempting to locate SCOM device...");
	sdev = get_device(DEV_SCOMM, DEV_SERIAL, -1);
	if (sdev == NULL) {
		puts("Failed to locate device, hopefully using the COM1 port works...");
	} else {
		/* Set configurations... */
		printf("Located device at %p...\n", sdev);
	}
}