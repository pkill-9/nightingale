/**
 *  Structures for maintaining lists of functions to be called on an
 *  interrupt.
 */

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#define NUM_IRQS                15

typedef struct irq_hook
{
    void (*handler) (struct irq_hook *hook);
    int irq;
    struct irq_hook *next;
}
irq_hook_t;

typedef void (*irq_handler_t) (struct irq_hook *hook);


#endif /** _INTERRUPTS_H */

/** vim: set ts=4 sw=4 et */
