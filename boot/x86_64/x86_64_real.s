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

.globl gdtdata

gdt:
	.quad 0
.equ gdtcode, (. - gdt)
	.word 0xFFFF
    .word 0x0000
    .byte 0
    .byte 0x9A
    .byte 0xCF
    .byte 0
.equ gdtdata, (. - gdt)
	.word 0xFFFF
    .word 0
    .byte 0
    .byte 0x92
    .byte 0xCF
    .byte 0
gdtp:
	.word (. - gdt - 1)
	.long gdt

.pseudo_idtp:
    .word 0
    .long 0

# Protected mode entry point.
.extern protected_mode_entry

.extern fakix_rsdt
.extern fakix_image_header
.extern fakix_memmap
.extern fakix_kern_start

.globl real_mode_entry
.type real_mode_entry, @function
real_mode_entry:
    # Load the kernel into physical memory
    call load_kernel

    # Find the Root System Descriptor Pointer (RSDP)
    call find_rsdp
    testw %si, %si
    jne error

    # Validate the RSDP we found.
    xorl %eax, %eax
    movw %es, %ax
    shll $4, %eax
    movl %eax, %edi
    movw $20, %si
    movw $36, %ax
    cmpb $2, 15(%edi) # ACPI v2 RSDP contains an extra 16 bytes of information.
    cmovew %ax, %si
    call chksum
    testb %al, %al
    jne error

    movl 16(%edi), %eax
    cmpw $2, 15(%edi)
    jne 1f
    movl 40(%edi), %eax
1:  movl %eax, fakix_rsdt

    # Scrounge as much useful BIOS information before disabling interrupts to
    # prepare for entering protected mode.
    call generate_mmap

    # We will reenable interrupts after we initialize our IDT in long mode
    cli

    call enable_a20_line

    # Disallow BIOS interrupts
    lidt .pseudo_idtp

    # Load the Global Descriptor Table
    lgdt gdtp
    
    # Set Protection Enable bit [CR0:0]
    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0

    ljmpl $gdtcode, $protected_mode_entry

/* The kernel entry point. */
.globl fakix_kernel_entry
fakix_kernel_entry:
    .long 0

load_fakix_kern_entry:
    movl $0x18, %esi
    movl fakix_kern_start(%esi), %esi
    movl %esi, fakix_kernel_entry
    movw $0x9090, 1f
    addl $0xC0, %ecx
    jmp 2f

# Load the FAKIX kernel into physical memory starting at fakix_kern_start.
load_kernel:
    movl $0, %edi
    movl $fakix_image_header, %ebx
    movl 0x10(%ebx), %ecx
    subl $0xC0, 0x18(%ebx)
.load_kernel_loop:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    pushl $0 # LBA [32:63]
    pushl %ecx # LBA [0:31]
    movl %edi, %esi
    shll $9, %esi
    leal fakix_kern_start(%esi), %esi
    shrl $4, %esi
    pushw %si
    pushw $0
    pushw $1
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc error
1:  jmp load_fakix_kern_entry
    incl %edi
    incl %ecx
2:  addl $0x10, %esp
    cmpl 0x18(%ebx), %edi
    jb .load_kernel_loop

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
    inb $0x92, %al
    orb $2, %al
    outb %al, $0x92
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

    cmpw $8, %si
    je .find_rsdp_loop_end
    movw $0, %si
    
    movw %es, %dx
    incw %dx
    movw %dx, %es
    jno .find_rsdp_loop

    movw $0xFFFF, %ax
    movw %ax, %si
    popw %ax
    ret

.find_rsdp_loop_end:
    movw $0, %si
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
    pushl %edi         # {
    xorb %al, %al      #     uint8_t sum = 0;
.chksum_loop:          #     while (cnt != 0) {
    addb (%edi), %al   #         sum += *sdt;
    incl %edi          #         sdt++;       
    decw %si           #         cnt--;
    jnz .chksum_loop   #     }
    popl %edi          #     return sum;
    ret                # }

error:
    hlt
