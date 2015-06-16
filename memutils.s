/**
 *  Assembly functions for fast memory copies.
 */

.section .text

/**********************************************************/

/**
 *  void memcopy (void *from, void *to, size_t count)
 *
 *  Copies the specified number of bytes.
 */
    .globl _memcopy
_memcopy:
    push    %ebp
    mov     %ebp, %esp
    push    %esi
    push    %edi
    push    %ecx

# register esi must point to the source; edi points to the destination and
# ecx contains the number of bytes to copy.
    mov     8(%ebp), %esi
    mov     12(%ebp), %edi
    mov     16(%ebp), %ecx

rep movsb

    pop     %ecx
    pop     %edi
    pop     %esi
    pop     %ebp
    ret

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
