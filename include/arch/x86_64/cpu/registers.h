#ifndef ARCH_X86_64_CPU_REGISTERS_H
#define ARCH_X86_64_CPU_REGISTERS_H 1

#include <stdint.h>

typedef uint64_t regval_t;

union registers {
    struct named_registers {
        regval_t rax;
        regval_t rbx;
        regval_t rcx;
        regval_t rdx;
        regval_t rdi;
        regval_t rsi;
        regval_t r8;
        regval_t r9;
        regval_t r10;
        regval_t r11;
        regval_t r12;
        regval_t r13;
        regval_t r14;
        regval_t r15;
        regval_t rsp;
        regval_t rbp;
    } named;

    struct offset_registers {
        regval_t registers[sizeof(struct named_registers) / sizeof(regval_t)];
    } array;

    struct system_call {
        regval_t syscall_number;
        regval_t rsvd0;
        regval_t return_address;
        regval_t third;
        regval_t first;
        regval_t second;
        regval_t fifth;
        regval_t sixth;
        regval_t fourth;
        regval_t rsvd1;
        regval_t rsvd2;
        regval_t rsvd3;
        regval_t rsvd4;
        regval_t rsvd5;
        regval_t rsvd6;
        regval_t rsvd7;
    } syscall_args;

    _Static_assert(sizeof(struct system_call) == sizeof(struct named_registers), 
                   "Improper number of fields in union structs [union registers]");
};


#endif