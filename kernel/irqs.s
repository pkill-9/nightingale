/**
 *  Hardware interrupt handlers.
 */

.section .text

/**********************************************************/

/**
 *  PRIVATE void save (void)
 *
 *  Saves general purpose registers on the stack. Since we do not support
 *  multiple processes, it is reasonable to save onto the kernel stack,
 *  and then restore from the same stack.
 */
_save:
    push    %ebp
    mov     %esp, %ebp
    cld
    pusha

# save segment registers.
    push    %ds
    push    %es
    push    %fs
    push    %gs

# when the interrupt handler does it's ret instruction, it will pop the
# address of the restore subroutine.
    push    _restore

# now we have to get the correct return address to go back to the irq
# handler that called us.
    mov     4(%ebp), %eax
    push    %eax
    ret

/**********************************************************/

/**
 *  PRIVATE void restore (void)
 *
 *  Restores the registers that were saved as part of handling an 
 *  interrupt. This is the reverse process to that of save().
 */
_restore:
    pop     %gs
    pop     %fs
    pop     %es
    pop     %ds
    popa

# we need to pop the ebp register that was saved as part of the function
# prologue in save.
    pop     %ebp

# skip over the return address that was pushed onto the stack when save was
# called.
    add     $4, %esp
    iret

/**********************************************************/

    .globl _irq0
_irq0:
    call    _save
    push    $0
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq1
_irq1:
    call    _save
    push    $1
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq2
_irq2:
    call    _save
    push    $2
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq3
_irq3:
    call    _save
    push    $3
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq4
_irq4:
    call    _save
    push    $4
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq5
_irq5:
    call    _save
    push    $5
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq6
_irq6:
    call    _save
    push    $6
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq7
_irq7:
    call    _save
    push    $7
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq8
_irq8:
    call    _save
    push    $8
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq9
_irq9:
    call    _save
    push    $9
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq10
_irq10:
    call    _save
    push    $10
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq11
_irq11:
    call    _save
    push    $11
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq12
_irq12:
    call    _save
    push    $12
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq13
_irq13:
    call    _save
    push    $13
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq14
_irq14:
    call    _save
    push    $14
    call    _call_handlers
    add     $4, %esp
    ret

    .globl _irq15
_irq15:
    call    _save
    push    $15
    call    _call_handlers
    add     $4, %esp
    ret

/**********************************************************/


/** vim: set ts=4 sw=4 et : */
