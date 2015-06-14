/**
 *  Entry point for a simple demo kernel.
 *
 *  This assumes use of a multiboot compliant loader, such as GRUB, and as
 *  such the initial environment should be:
 *  - 32 bit protected mode enabled.
 *  - interrupts disabled.
 *  - segment registers set up to have a base of 0 and limit of 4GiB.
 *  - ebx points to a multiboot info struct.
 *
 *  The purpose of this code is to set up the GDT and IDT; set up a stack
 *  and then hand over to the main C code.
 */

.section .text


/**********************************************************/

    .globl NIGHTINGALE
NIGHTINGALE:
    jmp     over_multiboot_header


.align 4

multiboot_header:
# multiboot header magic.
.long 0x1BADB002

# flags. Bits 0 and 1 set to request loading modules alligned on 4k 
# boundaries, and memory information to be included in the multiboot info
# structure.
.long 0x00000003

# checksum. The checksum, flags and magic must all sum to zero.
.long -(0x1BADB002 + 0x00000003)


over_multiboot_header:
# set up the stack for our kernel.
    mov     $STACK_BASE, %esp
    mov     %esp, %ebp

# initialise the GDT and IDT.
    call    _initialise_tables

    lgdt    _gdt
    lidt    _idt

    push    %ebx
    call    _main

/**********************************************************/

/**
 *  void idle (void)
 *
 *  Once the kernel has completed its task, execution will fall through
 *  to idle. This function puts the CPU in a low power state, and will
 *  never return, and serves to prevent the CPU from executing data in
 *  memory past the end of the kernel that is not intended to be executed.
 */
    .globl _idle
_idle:
    hlt
    jmp     _idle

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
