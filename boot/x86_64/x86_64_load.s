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

.include "load.h"

.globl load_entry
load_entry:
    cli

    ljmp $0, $.dummy
.dummy:

    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movl $FAKIX_REAL_MODE_STACK, %esp

    sti

    # 80h marks the start of the hard disk devices, going up to FFh.
    movb $0x80, %dl

    # Search for the hard drive that contains the Fakix partition table, which
    # is tagged by a magic constant equivalent to the qword-aligned and
    # null-terminated string "Fakix!".
.check_device:
    call read_sector
    
    call check_hdr
    cmpw $8, %ax
    je load_boot

    incb %dl
    jne .check_device

    hlt

# Load the Boot Module into memory.
load_boot:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    # Read from the first sector
    movl $FAKIX_IMAGE_HEADER, %ebx
    movl 0x44(%ebx), %ecx 
    pushl %ecx # LBA [32:63]
    movl 0x40(%ebx), %ecx
    pushl %ecx # LBA [0:31]
    pushw $FAKIX_BOOT
    pushw $0
    movw 0x48(%ebx), %cx
    pushw %cx
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc .read_failure
    addl $0x10, %esp

    jmp FAKIX_BOOT

.type check_hdr, @function
check_hdr:
    pushw %bx
    xorw %ax, %ax
    xorw %bx, %bx

.check_hdr_loop:
    movw FAKIX_IMAGE_HEADER(%bx), %ax
    cmpw %bx(.fakix_magik), %ax
    jne .end_loop

    incw %bx
    cmpw $8, %bx
    jne .check_hdr_loop

.check_hdr_ret:
    movw %bx, %ax
    popw %bx
    ret

# Read a single 512-byte block of data into fakix_image_header from the start of
# the current hard drive.
.type read_sector, @function
read_sector:
    # 42h is the BIOS int 13h opcode for 'Read Sectors' using logical block
    # addressing.
    movb $0x42, %ah
    # Read from the first sector
    pushl $0 # LBA [32:63]
    pushl $0 # LBA [0:31]
    pushw $FAKIX_IMAGE_HEADER
    pushw $0
    pushw $1
    pushw $0x10
    movw %sp, %si
    int $0x13
    jc .read_failure
    addl $0x10, %esp
    ret

.read_failure:
    hlt

.fakix_magik:
    .string "Fakix!"

bootloader_end:
.org FAKIX_LOADER_SIZE - 2
.word 0xAA55