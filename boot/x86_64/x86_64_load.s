#
# === x86_64_load.s ===
#
# Entry point: load_entry
#
# Scan devices for Fakix disk header, then load the rest of the boot module
# into memory.
#

.code16
.section .load

.extern fakix_real_stack
.extern fakix_image_header
.extern fakix_memmap

.globl load_entry
load_entry:
    cli

    ljmp $0, $1f
1:  xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movl $fakix_real_stack, %esp

    sti

    # 80h marks the start of the hard disk devices, going up to FFh.
    movb $0x80, %dl

    # Search for the hard drive that contains the Fakix partition table, which
    # is tagged by a magic constant equivalent to the qword-aligned and
    # null-terminated string "Fakix!".
.check_device:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    # Read from the first sector
    pushl $0 # LBA [32:63]
    pushl $1 # LBA [0:31]
    movl $fakix_image_header, %ebx
    shrl $4, %ebx
    pushw %bx
    pushw $0
    pushw $1
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc .read_failure
    addl $0x10, %esp
    
    call check_hdr
    cmpw $8, %ax
    je load_boot

    incb %dl
    jne .check_device

    hlt

.fakix_boot_offset:
    .long 0

load_fakix_boot_offset:
    movl $0x18, %esi
    movl fakix_boot(%esi), %esi
    movl %esi, .fakix_boot_offset
    # EEEEVVVVIIIIIILLLLL
    movw $0x9090, 1f
    addl $8, %ecx
    jmp 2f
    

# Load the Boot Module into memory.
load_boot:
    movl $0, %edi
    movl $fakix_image_header, %ebx
    movl 0x40(%ebx), %ecx
    subl $0x8, 0x48(%ebx)
.load_boot_loop:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    pushl $0 # LBA [32:63]
    pushl %ecx # LBA [0:31]
    movl %edi, %esi
    shll $9, %esi
    leal fakix_boot(%esi), %esi
    shrl $4, %esi
    pushw %si
    pushw $0
    pushw $1
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc .read_failure
1:  jmp load_fakix_boot_offset
    incl %edi
    incl %ecx
2:  addl $0x10, %esp
    cmpl 0x48(%ebx), %edi
    jb .load_boot_loop

    # FAKIX modules are actually just ELF64 executables. In every ELF header,
    # the entry address is at offset 0x18. The address is technically a quad,
    # but really it fits within a long, so we don't need to worry about whether
    # we truncated a 64-bit address or not.
    jmp *.fakix_boot_offset

.type check_hdr, @function
check_hdr:
    pushw %bx
    xorw %ax, %ax
    xorw %bx, %bx

.check_hdr_loop:
    movb fakix_image_header(%bx), %al
    cmpb .fakix_magik(%bx), %al
    jne .read_failure

    incw %bx
    cmpw $8, %bx
    jne .check_hdr_loop

.check_hdr_ret:
    movw %bx, %ax
    popw %bx
    ret

.read_failure:
    hlt

.fakix_magik:
    .string "Fakix!\0\0"

.org 510
.word 0xAA55
