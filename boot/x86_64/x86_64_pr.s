#
# === x86_64_pr.s ===
#
# Entry point: protected_mode_entry
#
# Sets up the paging system, among other preparations, and then jumps to long
# mode.
#

# This source runs in 32-bit protected mode.
.code32
.section .text

.extern fakix_pr_stack

.extern fakix_boot_pml4
.extern fakix_boot_pdp
.extern fakix_boot_pgdir
.extern fakix_boot_pgtbl

.extern fakix_kern_pml4
.extern fakix_kern_pdp
.extern fakix_kern_pgdir
.extern fakix_kern_pgtbl

# Long mode entry point.
.extern long_mode_entry
# Sadly, we cannot encapsulate the temporary GDT we use for entering protected
# mode, as flushing the data segment in real mode causes segmentation bugs so
# silent yet so deadly that it'll make the young systems programmer's blood run
# cold in fear of its very possibility.
.extern gdtdata

.globl fakix_gdt
.globl fakix_gdt_kern_code
.globl fakix_gdt_kern_data
.globl fakix_gdt_user_code
.globl fakix_gdt_user_data
.globl fakix_tss_descriptor
.globl fakix_interrupt_gate_descriptor

fakix_gdt:
# NULL selector
    .quad 0
.equ fakix_gdt_kern_code, (. - fakix_gdt)
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0x9A
    .byte 0xA0
    .byte 0x00
.equ fakix_gdt_kern_data, (. - fakix_gdt)
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0x92
    .byte 0xC0
    .byte 0x00
.equ fakix_gdt_user_code, (. - fakix_gdt)
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0xFA
    .byte 0xA0 
    .byte 0x00
.equ fakix_gdt_user_data, (. - fakix_gdt)
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0xF2
    .byte 0xC0
    .byte 0x00
.equ fakix_interrupt_gate_descriptor, (. - fakix_gdt)
    .quad 0
    .quad 0
.equ fakix_tss_descriptor, (. - fakix_gdt)
    .quad 0
    .quad 0
fakix_gdtp:
    .word (. - fakix_gdt - 1)
    .quad fakix_gdt

.globl protected_mode_entry
protected_mode_entry:
    # Flush GDT
    movw $gdtdata, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Switch to protected mode stack.
    movl $fakix_pr_stack, %esp
    pushl $0xCAFEBABE
    popl %eax
    xorl $0xCAFEBABE, %eax
    jne error

    # Check if we can use cpuid, and if long mode is available on this machine.
    call assert_cpuid_long_mode
    call enable_paging

    lgdt (fakix_gdtp)

    # Jump to long mode.
    ljmp $0x08, $long_mode_entry

assert_cpuid_long_mode:
    xorl %eax, %eax
    pushfl
	popl %eax

	movl %eax, %ecx
	xorl $0x200000, %eax

	pushl %eax
	popfl

	pushfl
	popl %eax

	pushl %ecx
	popfl

	cmpl %ecx, %eax
	je error

    # Assert that long mode is available
	movl $0x80000000, %eax
	cpuid
	cmpl $0x80000001, %eax
	jb error

	movl $0x80000001, %eax
	cpuid
	testl $(1 << 29), %edx
	jz error

	ret

error:
    hlt
    jmp error

# Set up the boot page table, the kernel page table, as well as the control 
# registers in preparation for jumping to long mode. The kernel page table is a
# subset of the boot page table, only containing virtual addresses 
# FAKIX_PHYSBASE and upwards. After this function is invoked, we will be in
# compatibility mode.
enable_paging:
    # Set up the kernel and boot page tables.
    call setup_boot_page_table
    call setup_kern_page_table

    # Clear the paging bit in CR0.
	movl %cr0, %eax
	andl $(~(1 << 31)), %eax
	movl %eax, %cr0

    # Set the boot page table as our active page table.
    movl $fakix_boot_pml4, %eax
    movl %eax, %cr3

    # Enable 48 bit logical addresses (PAE bit CR4:5).
	movl %cr4, %eax
	orl $(1 << 5), %eax
	movl %eax, %cr4

    # Read the value of the Extended Feature Enable Register, and set the
    # long mode enable bit and the system call extension bit. The latter
    # allows for the usage of the syscall/sysret instructions.
	movl $0xC0000080, %ecx
	rdmsr
	orl $((1 << 8) | (1 << 0)), %eax
	wrmsr

    # Set the enable paging bit (PGE bit CR0:31), thereby enabling paging.
	movl %cr0, %eax
	orl $(1 << 31), %eax
	movl %eax, %cr0

    ret

setup_boot_page_table:
    movl $fakix_boot_pdp, %eax
    orl $0x03, %eax
    movl %eax, (fakix_boot_pml4)

    movl $fakix_boot_pgdir, %eax
    orl $0x03, %eax
    movl %eax, (fakix_boot_pdp)

    movl $fakix_boot_pgtbl, %eax
    orl $0x03, %eax
    movl %eax, (fakix_boot_pgdir)

    xorl %ecx, %ecx
    .map_boot_pagetable:
        movl %ecx, %eax
		shll $12, %eax
		orl $0x83, %eax
		movl %eax, fakix_boot_pgtbl(, %ecx, 8)
		addl $1, %ecx
		cmpl $0x200, %ecx
		jl .map_boot_pagetable

    ret

setup_kern_page_table:
    movl $fakix_kern_pdp, %eax
    orl $0x03, %eax
    movl %eax, (0x800 + fakix_kern_pml4)

    # Shallow copy the kernel page table into the boot page table.
    movl $fakix_kern_pdp, %eax
    orl $0x03, %eax
    movl %eax, (0x800 + fakix_boot_pml4)

    movl $3, %edx

    xorl %ecx, %ecx
    .map_kern_pagetable:
        movl %ecx, %eax
		shll $6, %eax
        jo .setup_ret
		movl $0x83, fakix_kern_pdp(, %ecx, 8)
        movl %eax, fakix_kern_pdp(%edx, %ecx, 8)
		addl $1, %ecx
		cmpl $0x200, %ecx
		jl .map_kern_pagetable

    /*
    movl $fakix_kern_pgdir, %eax
    orl $0x03, %eax
    movl %eax, (fakix_kern_pdp)

    movl $fakix_kern_pgtbl, %eax
    orl $0x03, %eax
    movl %eax, (fakix_kern_pgdir)

    xorl %ecx, %ecx
    .map_kern_pagetable:
        movl %ecx, %eax
		shll $12, %eax
		orl $0x83, %eax
		movl %eax, fakix_kern_pgtbl(, %ecx, 8)
		addl $1, %ecx
		cmpl $0x200, %ecx
		jl .map_kern_pagetable
    */

.setup_ret:
    ret
