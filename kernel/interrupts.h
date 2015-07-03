/**
 *  Structures for maintaining lists of functions to be called on an
 *  interrupt.
 */

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

/**********************************************************/

// the standard architecture for PC's is to have two 8259 chips, each of
// which has 8 interrupt lines from hardware devices. However, the second
// 8259 chip is cascaded, ie. if it receives an interrupt, it will trigger
// an interrupt line on the first 8259, which will subsequently interrupt
// the CPU. That is why there are 15 hardware interrupts, not 16.
#define NUM_IRQS                15
#define CASCADE_IRQ             2

// the default IRQ vectors clash with the reserved vectors for exceptions.
// To get around this, we will tell the controllers to remap their IRQ
// vectors to be above the reserved vector range.
#define MASTER_BASE_VECTOR      0x20
#define SLAVE_BASE_VECTOR       0x28

// ports for the two interrupt controller chips.
#define MASTER_COMMAND          0x20
#define MASTER_DATA             0x21
#define SLAVE_COMMAND           0xA0
#define SLAVE_DATA              0xA1

// commands.
#define PIC_INITIALISE          0x10
#define WITH_ICW4               0x01
#define ICW4_8086               0x01
#define EOI                     0x20
#define READ_ISR                0x0B


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
