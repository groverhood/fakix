#include <io/dev.h>
#include <io/risky.h>
#include <paging/conv.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MCFG_SIG "MCFG"
#define MCFG_SIG_LENGTH (sizeof MCFG_SIG - 1)

packed struct mcfg_entry {
    uint64_t base;
    uint16_t groupnum;
    uint8_t startbus;
    uint8_t endbus;
    uint32_t rsvd;
};

static void enumerate_device(struct mcfg_entry *config);

void init_devices(struct fakix_sdt *rsdt)
{
    size_t headers = (rsdt->length - sizeof *rsdt) / sizeof(uint32_t);
    uint32_t *header_itr = (uint32_t *)(rsdt + 1);

    struct fakix_sdt *header;
    size_t i;
    for (i = 0; i < headers; ++i) {
        header = conv_phys_to_kern(header_itr[i]);
        if (!strncmp(header->signature, MCFG_SIG, MCFG_SIG_LENGTH)) {
            break;
        }
    }

    printf("Found MCFG header at %p...\n", header);

    struct mcfg_entry *mcfg_ent =
        (void *)((uint8_t *)header + sizeof *header + 8);

    puts("Enumerating devices...");
    size_t nentries =
        (header->length - (sizeof *header + 8)) / sizeof(struct mcfg_entry);
    printf("Iterating through %lx configuration address spaces, starting at "
           "%#lx...\n",
           nentries, mcfg_ent);
    while (nentries-- > 0) {
        enumerate_device(mcfg_ent++);
    }
}

#define DEV_MAX 48

static struct fakix_device devices[DEV_MAX];

struct fakix_device *get_device(enum dev_class dev_class, int dev_subclass,
                                int progif)
{
    struct fakix_device *dev = NULL;
    bool ignore_progif = (progif == -1);
    size_t i;
    for (i = 0; i < DEV_MAX; ++i) {
        struct fakix_device *enmdev = &devices[i];

        if (dev_class == enmdev->dclass && dev_subclass == enmdev->dsubclass &&
            (ignore_progif || enmdev->progif == progif)) {
            dev = enmdev;
            break;
        }
    }

    return dev;
}

static void check_bus(struct mcfg_entry *config_space, int bus);
static void check_device(struct mcfg_entry *config_space, int bus, int device);
static void check_function(struct mcfg_entry *config_space, int bus, int device,
                           int function);

static void enumerate_device(struct mcfg_entry *config_space)
{
    int bus;
    for (bus = config_space->startbus; bus <= config_space->endbus; ++bus) {
        check_bus(config_space, bus);
    }
}

#define MAX_DEVICES 32
#define MAX_FUNCTIONS 32

static void check_bus(struct mcfg_entry *config_space, int bus)
{
    int device;
    for (device = 0; device < MAX_DEVICES; ++device) {
        check_device(config_space, bus, device);
    }
}

packed struct mcfg_config_header {
    uint16_t devid;
    uint16_t vendorid;
    uint16_t command;
    uint16_t status;
    uint8_t revision;
    uint8_t progif;
    uint8_t dsubclass;
    uint8_t dclass;
    uint8_t cachlnsz;
    uint8_t latencytmr;
    uint8_t header_type;
    uint8_t bist;
    uint32_t bar0;
    uint32_t bar1;
};

static int get_vendor_id(struct mcfg_entry *config_space, int bus, int device,
                         int function);
static int get_class(struct mcfg_entry *config_space, int bus, int device,
                     int function);
static int get_subclass(struct mcfg_entry *config_space, int bus, int device,
                        int function);
static int get_progif(struct mcfg_entry *config_space, int bus, int device,
                      int function);
static int get_header_type(struct mcfg_entry *config_space, int bus, int device,
                           int function);
static int get_secondary_bus(struct mcfg_entry *config_space, int bus,
                             int device, int function);

static void check_device(struct mcfg_entry *config_space, int bus, int device)
{
    static size_t dev_bmp_ptr = 0;
    int function = 0;
    if (get_vendor_id(config_space, bus, device, function) != 0xFFFF) {
        if (dev_bmp_ptr < DEV_MAX) {
            struct fakix_device *dev = &devices[dev_bmp_ptr++];
            uintptr_t base = config_space->base;
            void *cfg =
                ((void *)(base + (((bus - config_space->startbus) << 20) |
                                  (device << 15) | (function << 12))));

            dev->cfg = cfg;
            dev->dclass = get_class(config_space, bus, device, function);
            dev->dsubclass = get_subclass(config_space, bus, device, function);
            dev->progif = get_progif(config_space, bus, device, function);
        }
    }
}
static int get_vendor_id(struct mcfg_entry *config_space, int bus, int device,
                         int function)
{
    uintptr_t base = config_space->base;
    struct mcfg_config_header *cfg =
        conv_phys_to_kern((base + (((bus - config_space->startbus) << 20) |
                                   (device << 15) | (function << 12))));

    return cfg->vendorid;
}

static int get_class(struct mcfg_entry *config_space, int bus, int device,
                     int function)
{
    uintptr_t base = config_space->base;
    struct mcfg_config_header *cfg =
        conv_phys_to_kern((base + (((bus - config_space->startbus) << 20) |
                                   (device << 15) | (function << 12))));

    return cfg->dclass;
}

static int get_subclass(struct mcfg_entry *config_space, int bus, int device,
                        int function)
{
    uintptr_t base = config_space->base;
    struct mcfg_config_header *cfg =
        conv_phys_to_kern((base + (((bus - config_space->startbus) << 20) |
                                   (device << 15) | (function << 12))));

    return cfg->dsubclass;
}

static int get_progif(struct mcfg_entry *config_space, int bus, int device,
                      int function)
{
    uintptr_t base = config_space->base;
    struct mcfg_config_header *cfg =
        conv_phys_to_kern((base + (((bus - config_space->startbus) << 20) |
                                   (device << 15) | (function << 12))));

    return cfg->progif;
}

static int get_header_type(struct mcfg_entry *config_space, int bus, int device,
                           int function)
{
    uintptr_t base = config_space->base;
    struct mcfg_config_header *cfg =
        conv_phys_to_kern((base + (((bus - config_space->startbus) << 20) |
                                   (device << 15) | (function << 12))));

    return cfg->header_type;
}

static int get_secondary_bus(struct mcfg_entry *config_space, int bus,
                             int device, int function)
{
    uintptr_t base = config_space->base;
    struct mcfg_config_header *cfg =
        conv_phys_to_kern((base + (((bus - config_space->startbus) << 20) |
                                   (device << 15) | (function << 12))));

    return *((uint8_t *)(cfg + 1) + 1);
}