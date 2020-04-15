#ifndef ARCH_ARM64_CPU_REGISTERS_H
#define ARCH_ARM64_CPU_REGISTERS_H 1

#include <stdint.h>

typedef uintptr_t regval_t;

union registers {
    struct named_registers {
        regval_t r0;
        regval_t r1;
        regval_t r2;
        regval_t r3;
        regval_t r4;
        regval_t r5;
        regval_t r6;
        regval_t r7;
        regval_t r8;
        regval_t r9;
        regval_t r10;
        regval_t r11;
        regval_t r12;
        regval_t r13;
        regval_t r14;
        regval_t r15;
        regval_t r16;
        regval_t r17;
        regval_t r18;
        regval_t r19;
        regval_t r20;
        regval_t r21;
        regval_t r22;
        regval_t r23;
        regval_t r24;
        regval_t r25;
        regval_t r26;
        regval_t r27;
        regval_t r28;
        regval_t r29;
        regval_t r30;
        regval_t r31;
    } named;

    struct offset_registers {
        regval_t registers[sizeof(struct named_registers) / sizeof(regval_t)];
    } array;

    struct system_call {
        regval_t arg0;
        regval_t arg1;
        regval_t arg2;
        regval_t arg3;
        regval_t arg4;
        regval_t arg5;
    } syscall_args;

    _Static_assert(sizeof(struct system_call) == sizeof(struct named_registers), 
                   "Improper number of fields in union structs [union registers]");
};

#endif