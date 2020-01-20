#ifndef FAKIX_IO_DEV_H
#define FAKIX_IO_DEV_H 1

#include <fakix.h>

enum dev_class {
	DEV_UNCLASSIFIED,
	DEV_STORAGE,
	DEV_NETWORK,
	DEV_DISPLAY,
	DEV_MULTIMEDIA,
	DEV_MEMORY,
	DEV_BRIDGE,
	DEV_SCOMM,
	DEV_BASESYSPERIPH,
	DEV_INPUT,
	DEV_DOCKING,
	DEV_PROCESSOR,
	DEV_SERIALBUS,
	DEV_WIRELESS,
	DEV_INTELLIGENT,
	DEV_SATELLITE,
	DEV_ENCRYPTION,
	DEV_SIGNAL,
	DEV_ACCELERATE,
	DEV_NONESSENTIAL,
	DEV_COPROCESSOR = 0x40,
	DEV_UNASSIGNED = 0xFF
};

/* Generic I/O device header. */
struct fakix_device {
	enum dev_class dclass;
	int dsubclass;
	int progif;
	void *cfg;
};

void init_devices(struct fakix_sdt *rsdt);

struct fakix_device *get_device(enum dev_class dev_class, int dev_subclass, int progif);

#endif