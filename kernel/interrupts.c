/**
 *  Code for initialising the 8259 Programmable Interrupt Controller (PIC)
 *  and for managing lists of functions (hooks) that are to be called on
 *  a particular interrupt.
 */

#include "interrupts.h"
#include "io.h"
#include "utils.h"

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

    while (hook != NULL)
    {
        (*hook->handler) (hook);
        hook = hook->next;
    }
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
