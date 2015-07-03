/**
 *  Structures for maintaining lists of functions to be called on an
 *  interrupt.
 */

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

/**********************************************************/

typedef struct irq_hook
{
    void (*handler) (struct irq_hook *hook);
    int irq;
    struct irq_hook *next;
}
irq_hook_t;

typedef void (*irq_handler_t) (struct irq_hook *hook);

/**********************************************************/

void pic_initialise (void);
void enable_irq (int irq);
void disable_irq (int irq);
void add_handler (irq_hook_t *hook, int irq, irq_handler_t handler);
void remove_handler (irq_hook_t *hook);
void call_handlers (int irq);

/**********************************************************/


#endif /** _INTERRUPTS_H */

/** vim: set ts=4 sw=4 et */
