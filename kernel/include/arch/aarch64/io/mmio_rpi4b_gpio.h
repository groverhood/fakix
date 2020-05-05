#ifndef KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_GPIO_H
#define KERNEL_ARCH_AARCH64_IO_MMIO_RPI4B_GPIO_H 1

#include <mfence.h>
#include <stdint.h>
#include <attributes.h>
#include <io/mmio_rpi4b_peripherals.h>

#define MMIO_RPI4B_GPIO_BASE (0x7e215000 - MMIO_RPI4B_LEGACY_PERIPHERAL_BASE)

#ifndef ASM_FILE

enum mmio_rpi4b_gpio_fsel {
    MMIO_RPI4B_GPIO_FSEL_INPUT,
    MMIO_RPI4B_GPIO_FSEL_OUTPUT,
    MMIO_RPI4B_GPIO_FSEL_ALT0 = 0b100,
    MMIO_RPI4B_GPIO_FSEL_ALT1 = 0b101,
    MMIO_RPI4B_GPIO_FSEL_ALT2 = 0b110,
    MMIO_RPI4B_GPIO_FSEL_ALT3 = 0b111,    
    MMIO_RPI4B_GPIO_FSEL_ALT4 = 0b011,
    MMIO_RPI4B_GPIO_FSEL_ALT5 = 0b010,
};

struct mmio_rpi4b_gpio_gpfsel0 {
    enum mmio_rpi4b_gpio_fsel fsel0 : 3;
    enum mmio_rpi4b_gpio_fsel fsel1 : 3;
    enum mmio_rpi4b_gpio_fsel fsel2 : 3;
    enum mmio_rpi4b_gpio_fsel fsel3 : 3;
    enum mmio_rpi4b_gpio_fsel fsel4 : 3;
    enum mmio_rpi4b_gpio_fsel fsel5 : 3;
    enum mmio_rpi4b_gpio_fsel fsel6 : 3;
    enum mmio_rpi4b_gpio_fsel fsel7 : 3;
    enum mmio_rpi4b_gpio_fsel fsel8 : 3;
    enum mmio_rpi4b_gpio_fsel fsel9 : 3;
    uint32_t                   rsvd : 2;
} packed;

struct mmio_rpi4b_gpio_gpfsel1 {
    enum mmio_rpi4b_gpio_fsel fsel10 : 3;
    enum mmio_rpi4b_gpio_fsel fsel11 : 3;
    enum mmio_rpi4b_gpio_fsel fsel12 : 3;
    enum mmio_rpi4b_gpio_fsel fsel13 : 3;
    enum mmio_rpi4b_gpio_fsel fsel14 : 3;
    enum mmio_rpi4b_gpio_fsel fsel15 : 3;
    enum mmio_rpi4b_gpio_fsel fsel16 : 3;
    enum mmio_rpi4b_gpio_fsel fsel17 : 3;
    enum mmio_rpi4b_gpio_fsel fsel18 : 3;
    enum mmio_rpi4b_gpio_fsel fsel19 : 3;
    uint32_t                    rsvd : 2;
} packed;

struct mmio_rpi4b_gpio_gpfsel2 {
    enum mmio_rpi4b_gpio_fsel fsel20 : 3;
    enum mmio_rpi4b_gpio_fsel fsel21 : 3;
    enum mmio_rpi4b_gpio_fsel fsel22 : 3;
    enum mmio_rpi4b_gpio_fsel fsel23 : 3;
    enum mmio_rpi4b_gpio_fsel fsel24 : 3;
    enum mmio_rpi4b_gpio_fsel fsel25 : 3;
    enum mmio_rpi4b_gpio_fsel fsel26 : 3;
    enum mmio_rpi4b_gpio_fsel fsel27 : 3;
    enum mmio_rpi4b_gpio_fsel fsel28 : 3;
    enum mmio_rpi4b_gpio_fsel fsel29 : 3;
    uint32_t                    rsvd : 2;
} packed;

struct mmio_rpi4b_gpio_gpfsel3 {
    enum mmio_rpi4b_gpio_fsel fsel30 : 3;
    enum mmio_rpi4b_gpio_fsel fsel31 : 3;
    enum mmio_rpi4b_gpio_fsel fsel32 : 3;
    enum mmio_rpi4b_gpio_fsel fsel33 : 3;
    enum mmio_rpi4b_gpio_fsel fsel34 : 3;
    enum mmio_rpi4b_gpio_fsel fsel35 : 3;
    enum mmio_rpi4b_gpio_fsel fsel36 : 3;
    enum mmio_rpi4b_gpio_fsel fsel37 : 3;
    enum mmio_rpi4b_gpio_fsel fsel38 : 3;
    enum mmio_rpi4b_gpio_fsel fsel39 : 3;
    uint32_t                    rsvd : 2;
} packed;

struct mmio_rpi4b_gpio_gpfsel4 {
    enum mmio_rpi4b_gpio_fsel fsel40 : 3;
    enum mmio_rpi4b_gpio_fsel fsel41 : 3;
    enum mmio_rpi4b_gpio_fsel fsel42 : 3;
    enum mmio_rpi4b_gpio_fsel fsel43 : 3;
    enum mmio_rpi4b_gpio_fsel fsel44 : 3;
    enum mmio_rpi4b_gpio_fsel fsel45 : 3;
    enum mmio_rpi4b_gpio_fsel fsel46 : 3;
    enum mmio_rpi4b_gpio_fsel fsel47 : 3;
    enum mmio_rpi4b_gpio_fsel fsel48 : 3;
    enum mmio_rpi4b_gpio_fsel fsel49 : 3;
    uint32_t                    rsvd : 2;
} packed;

struct mmio_rpi4b_gpio_gpfsel5 {
    enum mmio_rpi4b_gpio_fsel fsel50 : 3;
    enum mmio_rpi4b_gpio_fsel fsel51 : 3;
    enum mmio_rpi4b_gpio_fsel fsel52 : 3;
    enum mmio_rpi4b_gpio_fsel fsel53 : 3;
    enum mmio_rpi4b_gpio_fsel fsel54 : 3;
    enum mmio_rpi4b_gpio_fsel fsel55 : 3;
    enum mmio_rpi4b_gpio_fsel fsel56 : 3;
    enum mmio_rpi4b_gpio_fsel fsel57 : 3;
    enum mmio_rpi4b_gpio_fsel fsel58 : 3;
    enum mmio_rpi4b_gpio_fsel fsel59 : 3;
    uint32_t                    rsvd : 2;
} packed;

struct mmio_rpi4b_gpio_gpset {
    uint64_t setn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gpclr {
    uint64_t clrn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gplev {
    uint64_t levn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gpeds {
    uint64_t edsn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gpren {
    uint64_t renn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gpfen {
    uint64_t fenn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gphen {
    uint64_t henn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gplen {
    uint64_t lenn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gparen {
    uint64_t arenn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_gpafen {
    uint64_t afenn : 58;
    uint64_t rsvd : 6;
} packed;

struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg0 {
    uint32_t gpio00 : 2;
    uint32_t gpio01 : 2;
    uint32_t gpio02 : 2;
    uint32_t gpio03 : 2;
    uint32_t gpio04 : 2;
    uint32_t gpio05 : 2;
    uint32_t gpio06 : 2;
    uint32_t gpio07 : 2;
    uint32_t gpio08 : 2;
    uint32_t gpio09 : 2;
    uint32_t gpio10 : 2;
    uint32_t gpio11 : 2;
    uint32_t gpio12 : 2;
    uint32_t gpio13 : 2;
    uint32_t gpio14 : 2;
    uint32_t gpio15 : 2;
} packed;

struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg1 {
    uint32_t gpio16 : 2;
    uint32_t gpio17 : 2;
    uint32_t gpio18 : 2;
    uint32_t gpio19 : 2;
    uint32_t gpio20 : 2;
    uint32_t gpio21 : 2;
    uint32_t gpio22 : 2;
    uint32_t gpio23 : 2;
    uint32_t gpio24 : 2;
    uint32_t gpio25 : 2;
    uint32_t gpio26 : 2;
    uint32_t gpio27 : 2;
    uint32_t gpio28 : 2;
    uint32_t gpio29 : 2;
    uint32_t gpio30 : 2;
    uint32_t gpio31 : 2;
} packed;

struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg2 {
    uint32_t gpio32 : 2;
    uint32_t gpio33 : 2;
    uint32_t gpio34 : 2;
    uint32_t gpio35 : 2;
    uint32_t gpio36 : 2;
    uint32_t gpio37 : 2;
    uint32_t gpio38 : 2;
    uint32_t gpio39 : 2;
    uint32_t gpio40 : 2;
    uint32_t gpio41 : 2;
    uint32_t gpio42 : 2;
    uint32_t gpio43 : 2;
    uint32_t gpio44 : 2;
    uint32_t gpio45 : 2;
    uint32_t gpio46 : 2;
    uint32_t gpio47 : 2;
} packed;

struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg3 {
    uint32_t gpio48 : 2;
    uint32_t gpio49 : 2;
    uint32_t gpio50 : 2;
    uint32_t gpio51 : 2;
    uint32_t gpio52 : 2;
    uint32_t gpio53 : 2;
    uint32_t gpio54 : 2;
    uint32_t gpio55 : 2;
    uint32_t gpio56 : 2;
    uint32_t gpio57 : 2;
    uint32_t rsvd  : 12;
} packed;

struct mmio_rpi4b_gpio {
    struct mmio_rpi4b_gpio_gpfsel0 gpfsel0;
    struct mmio_rpi4b_gpio_gpfsel0 gpfsel1;
    struct mmio_rpi4b_gpio_gpfsel0 gpfsel2;
    struct mmio_rpi4b_gpio_gpfsel0 gpfsel3;
    struct mmio_rpi4b_gpio_gpfsel0 gpfsel4;
    struct mmio_rpi4b_gpio_gpfsel0 gpfsel5;

    struct mmio_rpi4b_gpio_gpset  gpset;
    struct mmio_rpi4b_gpio_gpclr  gpclr;
    struct mmio_rpi4b_gpio_gplev  gplev;
    struct mmio_rpi4b_gpio_gpeds  gpeds;
    struct mmio_rpi4b_gpio_gpren  gpren;
    struct mmio_rpi4b_gpio_gplen  gplen;
    struct mmio_rpi4b_gpio_gparen gparen;
    struct mmio_rpi4b_gpio_gpafen gpafen;
    
    struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg0 ppc0;
    struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg1 ppc1;
    struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg2 ppc2;
    struct mmio_rpi4b_gpio_pup_pdn_cntrl_reg3 ppc3;
} packed;

#endif

#endif