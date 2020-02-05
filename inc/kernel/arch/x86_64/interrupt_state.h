#ifndef FAKIX_ARCH_X86_64_INTERRUPT_STATE_H
#define FAKIX_ARCH_X86_64_INTERRUPT_STATE_H 1

struct interrupt_state {

};

struct fault_state {
	
};

#define ireturn do { \
		__asm__ __volatile__ ("iretq"); \
		__builtin_unreachable(); \
	} while (0)

#endif