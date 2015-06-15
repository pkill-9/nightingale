/**
 *  Functions for building GDT and IDT descriptors.
 */

#include "stdint.h"
#include "descriptors.h"
#include "utils.h"

/**********************************************************/

/**
 *  Fill in the given GDT entry with the base, limit and attribute fields
 *  specified.
 *
 *  Note that it is not necessary to specify the PRESENT bit in the access
 *  bitmap, as this function will set it regardless.
 */
    PUBLIC void
make_gdt_entry (entry, base, limit, flags, access_bits)
    struct gdt_entry *entry;    // GDT entry struct to be filled in
    uint32_t base;              // base offset for the segment
    uint32_t limit;             // length of the segment
    uint8_t flags;              // GDT flags field
    uint8_t access_bits;        // GDT access field
{
    entry->base_low = (uint16_t) base & 0xFFFF;
    entry->base_mid = (uint8_t) (base >> 16) & 0xFF;
    entry->base_high = (uint8_t) (base >> 24) & 0xFF;

    entry->limit_low = (uint16_t) limit & 0xFFFF;
    entry->flags_and_limit_high = (uint8_t) (limit >> 16) & 0x0F | 
        (flags & 0x0F);

    entry->access_bits = access_bits | GDT_PRESENT (1);
}

/**********************************************************/

/**
 *  Fill in a given IDT entry structure with the given parameters. This
 *  function will have less bit manipulation compared to the GDT entry
 *  builder, since the IDT entry layout is not as fragmented.
 *
 *  Once again, this function will automatically set the PRESENT bit on
 *  the IDT descriptor being filled in.
 */
    PUBLIC void
make_idt_entry (entry, handler_address, segment_selector, attributes)
    struct idt_entry *entry;    // IDT entry struct to be filled in
    uint32_t handler_address;   // address of interrupt handler
    uint16_t segment_selector;  // should be the kernel code segment
    uint8_t attributes;         // gate type and priv level
{
    entry->unused = 0;

    entry->handler_low = (uint16_t) handler_address & 0xFFFF;
    entry->handler_high = (uint16_t) (handler_address >> 16) & 0xFFFF;

    entry->segment_selector = segment_selector;
    entry->attributes = attributes | IDT_PRESENT (1);
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
