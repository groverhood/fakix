#ifndef FAKIX_LIBC_ATTRIBUTES_H
#define FAKIX_LIBC_ATTRIBUTES_H

/* Use this with architecture-specific structures. */
#define packed __attribute__((packed))

/* Attach this macro to any function that you don't intend on implementing in
   the kernel. */
#ifdef __FAKIX_KERNEL__
#define kernel_unfriendly \
    __attribute__((error ("This function cannot be invoked in the kernel.")))
#else
#define kernel_unfriendly
#endif

#define align(X) __attribute__((aligned (X)))

#endif