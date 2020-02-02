#ifndef FAKIX_SYN_SPINLOCK_H
#define FAKIX_SYN_SPINLOCK_H 1

#include <stdint.h>
#include <stdbool.h>

typedef bool spinlock_cond_t(void *aux);

/* Ownerless lock. */
struct spinlock {
	uint64_t count;
};

struct owned_spinlock {
	uint64_t count;
	int holder; /* UID for holder. This integer is left to be interpreted as
                   what the user needs. */
};

/* void ownerless_spinlock_inline_acquire(struct spinlock *plck, CONDITIONAL cond); */
#define ownerless_spinlock_inline_acquire(plck, cond) \
	do { \
		uint64_t count = 0, maxcount = (plck)->count; \
		while (count++ < maxcount && (cond));\
	} while (0)

/* void owned_spinlock_inline_acquire(struct owned_spinlock *plck, CONDITIONAL cond, int newholder); */
#define owned_spinlock_inline_acquire(plck, cond, newholder) \
    do { \
        ownerless_spinlock_inline_acquire(plck, cond); \
        (plck)->holder = newholder;\
    } while (0)

#define spinlock_init(plck, cnt) \
    do { \
        memset((plck), 0, sizeof *(plck)); \
        (plck)->count = cnt;\
    } while (0)\


#endif