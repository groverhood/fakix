#
# === x86_64_real.s ===
#
# Entry point: real_mode_entry
#
# Uses BIOS goodies to provide the Kernel process with runtime information,
# then jumps to protected mode.
#

# This code runs in 16-bit real mode.
.code16
.section .text

# Nice constants.
#include "load.h"

# Protected mode entry point.
.extern protected_mode_entry

.extern fakix_rsdt
.extern fakix_image_header
.extern fakix_memmap
.extern fakix_kern_start


.globl real_mode_entry
.type real_mode_entry, @function
real_mode_entry:
    call find_rsdp
    testw %si, %si
    jne error

    # Validate the RSDP we found.
    movw %ax, %di
    movw $20, %si
    movw $36, %ax
    cmpw $2, 15(%di) # ACPI v2 RSDP contains an extra 16 bytes of information.
    cmovew %ax, %si
    call chksum
    testb %al, %al
    jne error

    movl 16(%di), %eax
    cmpw $2, 15(%di)
    jne 1f
    movl 40(%di), %eax
1:  movl %eax, fakix_rsdt

    # Scrounge as much useful BIOS information before disabling interrupts to
    # prepare for entering protected mode.
    call generate_mmap
    
    # Enable the A20 line so we can load the kernel into high memory.
    call enable_a20_line

    # Load the kernel into physical memory
    call load_kernel

    # Set Protection Enable bit [CR0:0]
    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0

    ljmp $0x8, $protected_mode_entry

# Load the FAKIX kernel into physical memory starting at fakix_kern_start.
load_kernel:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    # Read from the first sector
    movl $fakix_image_header, %ebx
    movl 0x14(%ebx), %ecx 
    pushl %ecx # LBA [32:63]
    movl 0x10(%ebx), %ecx
    pushl %ecx # LBA [0:31]
    movl $fakix_kern_start, %ecx
    pushw %cx
    shrl $4, %ecx
    pushw %cx
    movw 0x18(%ebx), %cx # Number of sectors to read
    pushw %cx
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc error
    addl $0x10, %esp
    ret

# Generate the FAKIX memory map at fakix_memmap. This doesn't perform anything
# on the returned list. During kernel initialization, it is expected that some
# sorting + coalescing of overlapping regions will be performed.
generate_mmap:
    xorw %ax, %ax
    movw %ax, %es
    xorl %ebx, %ebx
    movl $0x534D4150, %edx
    movw $fakix_memmap, %di

.generate_mmap_loop:
    movl $0xE820, %eax
    movl $24, %ecx
    movl $1, %es:16(%di)
    int $0x15

    cmpl $0x534D4150, %eax
    jne error

    jc .generate_mmap_loop_end
    addw $24, %di
    cmpw $0x600, %di
    jb .generate_mmap_loop

.generate_mmap_loop_end:
    ret

# Enable the A20 line, which will allow for addressing above 1MB.
enable_a20_line:
    cli
    inb $0x92, %al
    orb $2, %al
    outb %al, $0x92
    sti
    ret

# What we identify the RSDP by.
.rsdp_identifier:
    .string "RSD PTR "

#
# Find the RSDP located between 0xE000:0 and 0xFFFF:F
#
# %es:%si - The segment pointer to the found RSDP. The offset register %si will
#           always be zero, unless an error was found, in which case it will be
#           0xFFFF. Therefore, it is worth treating this value as an indicator 
#           of the search's success, or conversely of the search's failure.
#
find_rsdp:
    pushw %ax
    movw $0xE000, %ax
    movw %ax, %es
    movw $0, %si

.find_rsdp_loop:

    .find_rsdp_cmp:
        movb %es:(%si), %ah
        movb .rsdp_identifier(%si), %al
        cmpb %al, %ah
        jne .find_rsdp_cmp_end
        incw %si
        cmpw $8, %si
        jb .find_rsdp_cmp
    .find_rsdp_cmp_end:

    cmpb %al, %ah
    movw $0, %si
    je .find_rsdp_loop_end
    
    pushw %ax
    movw %es, %ax
    incw %ax
    movw %ax, %es
    popw %ax
    jnc .find_rsdp_loop

.find_rsdp_loop_end:
    movw $0xFFFF, %ax
    cmovcw %ax, %si
    popw %ax
    ret

#
# Validate SDT-based structures by summing their bytes and asserting that the
# result & 0xFF is equal to zero.
#
# %di - The SDT-based structure.
# %si - The size in bytes of the structure pointed to by %di.
# %al - Contains the sum & 0xFF after the procedure is invoked.
#
chksum:                # uint8_t chksum(uint8_t *sdt, uint16_t bytes)
                       # {
    xorb %al, %al      #     uint8_t sum = 0;
.chksum_loop:          #     while (cnt != 0) {
    addb (%di), %al    #         sum += *sdt;
    incw %di           #         sdt++;       
    decw %si           #         cnt--;
    jne .chksum_loop   #     }
    ret                #     return sum;
                       # }

error:
    hlt
