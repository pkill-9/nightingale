/**
 *  Code for initialising the 8259 Programmable Interrupt Controller (PIC)
 *  and for managing lists of functions (hooks) that are to be called on
 *  a particular interrupt.
 */

#include "interrupts.h"
#include "io.h"
#include "stdint.h"
#include "utils.h"

/**********************************************************/

PRIVATE void end_of_interrupt (int irq);
PRIVATE bool spurious_interrupt (int irq);


/**********************************************************/

// array of hook lists. When an IRQ comes in, we will get the hook list
// using the IRQ number as an index, then step along the list and run all
// the hook functions.
PRIVATE irq_hook_t *irq_hooks [NUM_IRQS];


/**********************************************************/

/**
 *  Initialise the PIC hardware. By default, IRQ's 0 thru 7 are mapped to
 *  CPU interrupt 8 thru 15, which conflicts with the 32 protected mode
 *  exceptions. As a result, this function will tell the PIC to remap 
 *  these IRQ's so that there is no clash.
 */
    PUBLIC void
pic_initialise (void)
{
    // Note that this function does not reenable interrupts before it 
    // returns. The return-from-interrupt assembly statement does this
    // automatically.
    disable_interrupts ();

    // we start with no handlers registered.
    for (int i = 0; i < NUM_IRQS; i ++)
        irq_hooks [i] = NULL;

    // initialise the interrupt controller chips.
    outb (MASTER_COMMAND, PIC_INITIALISE | WITH_ICW4);
    outb (SLAVE_COMMAND, PIC_INITIALISE | WITH_ICW4);
    outb (MASTER_DATA, MASTER_BASE_VECTOR);
    outb (SLAVE_DATA, SLAVE_BASE_VECTOR);

    // tell the controllers that there is a slave PIC at IRQ2
    outb (MASTER_DATA, 4);
    outb (SLAVE_DATA, 2);

    outb (MASTER_DATA, ICW4_8086);
    outb (SLAVE_DATA, ICW4_8086);

    // set the initial interrupt mask, with everything disabled, except
    // for the cascade IRQ. When devices are initialised, they will enable
    // their IRQ line separately.
    outb (MASTER_DATA, 0xFF & ~(1 << CASCADE_IRQ));
    outb (SLAVE_DATA, 0xFF);
}

/**********************************************************/

/**
 *  Enable the specified IRQ line by clearing the bit in the corresponding
 *  controller's interrupt mask register.
 */
    PUBLIC void
enable_irq (irq)
    int irq;                    // between 0 and 15.
{
    uint16_t mask_port;
    uint8_t imr;

    // choose which controller based on the IRQ.
    mask_port = (irq > 7)? SLAVE_DATA : MASTER_DATA;

    // if the IRQ is on the slave controller, figure out which IRQ line on
    // the slave is it. 8 lines per controller chip, so...
    irq %= 8;

    imr = inb (mask_port);

    // if the IRQ is already enabled, we can skip a slow port operation.
    if (imr & (1 << irq) == 0)
        return;

    outb (mask_port, imr & ~(1 << irq));
}

/**********************************************************/

/**
 *  Disable an interrupt by setting the corresponding interrupt mask 
 *  register bit.
 */
    PUBLIC void
disable_irq (irq)
    int irq;                    // between 0 and 15
{
    uint16_t mask_port;
    uint8_t imr;

    // most of this is very similar to enable_irq, but we want to set the
    // bit in the mask register, not clear it.
    mask_port = (irq > 7)? SLAVE_DATA : MASTER_DATA;
    irq %= 8;

    imr = inb (mask_port);

    // test if the bit is already set.
    if (imr & (1 << irq) != 0)
        return;

    outb (mask_port, imr | (1 << irq))
}

/**********************************************************/

/**
 *  Inserts a new hook into the list for the given IRQ.
 */
    PUBLIC void
add_handler (hook, irq, handler)
    irq_hook_t *hook;           // points to a hook structure to fill in
    int irq;                    // IRQ line to hook into
    irq_handler_t handler;      // handler function to be called
{
    // fill in the hook struct.
    hook->handler = handler;
    hook->irq = irq;
    hook->next = irq_hooks [irq];

    // insert the hook on the start of the linked list
    irq_hooks [irq] = hook;
}

/**********************************************************/

/**
 *  Remove a given handler from the hook list. Note that this requires us
 *  to do a linear search over the list to find the hook to unlink.
 */
    PUBLIC void
remove_handler (hook)
    irq_hook_t *hook;           // hook struct to unlink
{
    irq_hook_t **prev = irq_hooks + hook->irq;

    while (*prev != NULL)
    {
        if (*prev == hook)
        {
            *prev = (*prev)->next;
            return;
        }

        prev = &(*prev)->next;
    }
}

/**********************************************************/

/**
 *  This function is called from each hardware interrupt service routine,
 *  and will step through the hook list for the given IRQ and call each
 *  hook function.
 */
    PUBLIC void
call_handlers (irq)
    int irq;                    // PIC interrupt line that triggered
{
    irq_hook_t *hook = irq_hooks [irq];

    // sometimes an interrupt signal goes away before the CPU has time to
    // respond to the interrupt. In this case, the controller will give
    // the CPU a fake interrupt number. This function will detect if the
    // IRQ that was triggered is a genuine interrupt. If the interrupt is
    // spurious, we do not need to send an EOI to the controller, or run
    // any handler, so we will just return to the service routine.
    //
    // Spurious interrupts can be caused by software sending EOI at the
    // wrong time, or less commonly noise on the interrupt lines.
    if (spurious_interrupt (irq))
        return;

    while (hook != NULL)
    {
        (*hook->handler) (hook);
        hook = hook->next;
    }

    end_of_interrupt (irq);
}

/**********************************************************/

/**
 *  Sends the end of interrupt command to the relevant interrupt 
 *  controller chips. For interrupts from the master chip, we only need to
 *  send the EOI to the master; if the interrupt came from the slave, we
 *  need to send it to both controllers.
 */
    PRIVATE void
end_of_interrupt (irq)
    int irq;                    // irq line that was triggered
{
    if (irq >= 8)
        outb (SLAVE_COMMAND, EOI);

    outb (MASTER_COMMAND, EOI);
}

/**********************************************************/

/**
 *  Test if the interrupt that just occurred was a spurious interrupt.
 *  Sometimes if there is noise on an IRQ line, or software issues an EOI
 *  command at the wrong time, the interrupt controller will detect an
 *  interrupt signal, but it disappears before the CPU has time to 
 *  respond. In this case, the controller will report that the interrupt
 *  came from the lowest priority line on the chip (IRQ 7 or 15 for master
 *  and slave respectively). We can tell if the interrupt is spurious by
 *  checking the controller's In Service Register: this register has a bit
 *  for each IRQ on the controller, which is set when the IRQ is being
 *  serviced. In the case of spurious interrupts, the ISR bit will be
 *  clear.
 */
    PRIVATE bool
spurious_interrupt (irq)
    int irq;                    // can be any IRQ line from 0 to 15.
{
    uint16_t command_port;
    uint8_t isr;

    // spurious interrupts only happen on IRQ 7 or 15, so any other IRQ
    // will be considered to be genuine.
    if (irq != 7 || irq != 15)
        return false;

    // do we want the ISR from the master controller or the slave?
    command_port = (irq == 7)? MASTER_COMMAND : SLAVE_COMMAND;

    // get the contents of the in service register.
    outb (command_port, READ_ISR);
    isr = inb (command_port);

    // check bit 7, corresponding to the lowest priority IRQ.
    if ((isr & 0x80) != 0)
    {
        // one slight complication is that if the spurious interrupt came
        // from the slave controller, the slave will not expect an EOI,
        // but the master will still expect an EOI, since it only knows
        // that the slave raised an IRQ.
        if (irq == 15)
            outb (MASTER_COMMAND, EOI);

        return true;
    }

    return false;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
