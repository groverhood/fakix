#ifndef MFENCE_H
#define MFENCE_H 1

#define mfence() asm volatile ("dmb ishld" ::: "memory")

#endif