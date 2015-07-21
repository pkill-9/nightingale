/**
 *  Functions for setting up the global descriptor table and the interrupt
 *  descriptor table.
 */

#include "protect.h"
#include "stdint.h"
#include "descriptors.h"
#include "utils.h"

/**********************************************************/

PRIVATE void flat_gdt (void);
PRIVATE void empty_idt (void);

/**********************************************************/

/**
 *  Initialise the gdt and idt tables declared in protect.h, and set the
 *  values for the CPU GDT and IDT registers.
 */
    PUBLIC void
initialise_tables (void)
{
    flat_gdt ();
    empty_idt ();

    gdtr.base_address = (uint32_t) &gdt;
    gdtr.size = sizeof (struct gdt_entry) * NUM_GDT_ENTRIES;

    idtr.base_address = (uint32_t) &idt;
    idtr.size = sizeof (struct idt_entry) * NUM_IDT_ENTRIES;
}

/**********************************************************/

/**
 *  Initialise the IDT entry at offset given by vector to invoke the
 *  handler at the specified address. Note that this function will modify
 *  the IDT entry, and will set the present bit. The caller privilege
 *  level and gate type will also be set to the values specified in this
 *  functions arguments.
 */
    PUBLIC void
set_interrupt_gate (vector, handler, privilege, gate_type)
    uint8_t vector;             // offset into the IDT
    void *handler;              // address of handler function
    uint8_t privilege;          // ring level required to use the trap
    uint8_t gate_type;          // hardware irq, or software trap
{
    // set the gate privilege and type.
    idt [vector].attributes |= (privilege << 7) & 0x70;
    idt [vector].attributes |= gate_type & 0x0F;

    // store the 32 bit address of the handler.
    idt [vector].handler_low = (int) handler & 0x0000FFFF;
    idt [vector].handler_high = ((int) handler >> 16) & 0x0000FFFF;

    // make sure that the present bit of the specified IDT entry is set.
    idt [vector].attributes |= IDT_PRESENT;
}

/**********************************************************/

/**
 *  Initialise the GDT with 3 entries, the null entry which is always
 *  kept in GDT[0], and code and data segments in 1 and 2. The code and
 *  data segments will have a base of 0 and limit of 4 GiB, hence the name
 *  flat gdt.
 */
    PRIVATE void
flat_gdt (void)
{
    // first entry in the GDT (GDT[0]) cannot be used, and we will set
    // the base and limit to 0.
    make_gdt_entry (&gdt [0], 0, 0, 0, 0);

    // kernel code segment.
    make_gdt_entry (&gdt [1], 0, 0xFFFFF, 
      GDT_GRANULARITY (1) | GDT_SIZE (1),
      GDT_PRESENT (1) | GDT_RING_LEVEL (0) | GDT_EXECUTABLE (1) | 
      GDT_READ_WRITE (1));

    // kernel data segment.
    make_gdt_entry (&gdt [2], 0, 0xFFFFF,
      GDT_GRANULARITY (1) | GDT_SIZE (1),
      GDT_PRESENT (1) | GDT_RING_LEVEL (0) | GDT_READ_WRITE (1));
}

/**********************************************************/

/**
 *  Create an empty interrupt descriptor table by stepping through all
 *  the entries and clear the present bit in each one.
 */
    PRIVATE void
empty_idt (void)
{
    for (int i = 0; i < NUM_IDT_ENTRIES; i ++)
        idt [i].attributes &= ~IDT_PRESENT;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
