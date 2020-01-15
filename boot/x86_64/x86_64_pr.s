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

#include "load.h"

.gdt:
	.quad 0
.equ .gdtcode, (. - .gdt)
	.quad (1 << 44) | (1 << 47) | (1 << 41) | (1 << 43) | (1 << 53)
.equ .gdtdata, (. - .gdt)
	.quad (1 << 44) | (1 << 47) | (1 << 41)
# We will modify this later on.
.equ .gdttss, (. - .gdt)
	.quad 0
	.quad 0
.gdtp:
	.word (. - .gdt - 1)
	.quad .gdt

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

.globl protected_mode_entry
protected_mode_entry:
    # Switch to protected mode stack.
    movl $fakix_pr_stack, %esp

    # Check if we can use cpuid, and if long mode is available on this machine.
    call assert_cpuid_long_mode
    call enable_paging

    # Load the Global Descriptor Table
    lgdt (.gdtp)
    movl $.gdtdata, %eax
	movw %ax, %ss
	movw %ax, %ds
	movw %ax, %es

    # Jump to long mode.
    ljmp $0x08, $long_mode_entry

assert_cpuid_long_mode:
    ret

# Set up the boot page table, the kernel page table, as well as the control 
# registers in preparation for jumping to long mode. The kernel page table is a
# subset of the boot page table, only containing virtual addresses 
# FAKIX_PHYSBASE and upwards. After this function is invoked, we will be in
# compatibility mode.
enable_paging:
    # Set up the kernel and boot page tables.
    call setup_boot_page_table
    call setup_kern_page_table

    # Clear CR0. This turns off protected mode.
	movl %cr0, %eax
	andl $(~(1 << 31)), %eax
	movl %eax, %cr0

    # Set the boot page table as our active page table
    movl $fakix_boot_pml4, %eax
    movl %eax, %cr3

    # Enable 48 bit physical addresses (PAE bit CR4:5)
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
    movl %eax, fakix_boot_pml4

    movl $fakix_boot_pgdir, %eax
    orl $0x03, %eax
    movl %eax, fakix_boot_pdp

    movl $fakix_boot_pgtbl, %eax
    orl $0x83, %eax
    movl %eax, fakix_boot_pgdir

    xorl %ecx, %ecx
    .map_boot_pagetable:
        movl %ecx, %eax
		shll $11, %eax
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

    movl $fakix_kern_pgdir, %eax
    orl $0x03, %eax
    movl %eax, fakix_kern_pdp

    movl $fakix_kern_pgtbl, %eax
    orl $0x83, %eax
    movl %eax, fakix_kern_pgdir

    xorl %ecx, %ecx
    .map_kern_pagetable:
        movl %ecx, %eax
		shll $11, %eax
		orl $0x83, %eax
		movl %eax, fakix_kern_pgtbl(, %ecx, 8)
		addl $1, %ecx
		cmpl $0x200, %ecx
		jl .map_kern_pagetable

    ret
