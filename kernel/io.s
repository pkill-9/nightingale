/**
 *  Wrapper functions so that C functions can make use of the CPU
 *  instructions for reading and writing from ports.
 */

.section .text

/**********************************************************/

/**
 *  void outb (uint16_t port, uint8_t value)
 *
 *  Writes the given value on the given port.
 */
    .globl _outb
_outb:
    push    %ebp
    mov     %esp, %ebp
    push    %edx

# for the outb instruction, the port goes in register dx and the value
# in register al
    mov     8(%ebp), %dx
    mov     10(%ebp), %al
    outb    %al, %dx

    pop     %edx
    pop     %ebp
    ret

/**********************************************************/

/**
 *  uint8_t inb (uint16_t port)
 *
 *  Reads a byte from the specified IO port and returns it.
 */
    .globl _inb
_inb:
    push    %ebp
    mov     %esp, %ebp
    push    %edx

# clear eax register of any existing data.
    xor     %eax, %eax

# same operands as for outb; port in dx register, value will be read into
# al.
    mov     8(%ebp), %dx
    inb     %dx, %al

    pop     %edx
    pop     %ebp
    ret

/**********************************************************/

/**
 *  void disable_interrupts (void)
 *
 *  disables most interrupts by executing the cli instruction, then
 *  returns.
 */
    .globl _disable_interrupts
_disable_interrupts:
    push    %ebp
    mov     %esp, %ebp

    cli

    leave
    ret

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
