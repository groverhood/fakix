/**
 *  === syscall.s ===
 * 
 *  This source implements the initialization process for x86_64 system calls.
 *  The syscall/sysret instructions have already been enabled by the boot
 *  module, so all that is left to do is write the address of our syscall stub,
 *  sys_entry, into IA32_LSTAR, as well as writing selectors into IA32_STAR.
 **/

.extern syscall_table

.type sys_entry, @function
sys_entry:
	/* Save important state stored in %r11 (rflags) and %rcx (next instruction
	   after trap). */
	pushq %r11
	pushq %rcx

	/* Shift arguments back into x86_64 calling convention. */
	movq %r8, %rcx
	movq %r9, %r8
	movq %r10, %r9

	/* Invoke system call handler. */
	callq *syscall_table(, %rax, 8)

	popq %rcx
	popq %r11
	sysret

/* void syscall_init(void); */
.globl syscall_init
.type syscall_init, @function
syscall_init:
	/* Write the address of sys_entry into IA32_LSTAR. */
	movl $0xC0000082, %ecx
	movq $sys_entry, %rax
	movq $sys_entry, %rdx
	shrq $32, %rdx
	wrmsr

	/* Write selectors used in syscall/sysret into IA32_STAR. */
	movl $0xC0000081, %ecx
	movl $((24 << 16) | (8 << 0)), %edx
	xorl %eax, %eax
	wrmsr

	retq