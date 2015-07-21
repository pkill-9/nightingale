/**
 *  Definitions for the two protected mode data structures - the interrupt
 *  descriptor table (IDT) and the global descriptor table (GDT).
 */

#ifndef _DESCRIPTORS_H
#define _DESCRIPTORS_H

#include "stdint.h"

/**********************************************************/

/**
 *  The CPU has two special purpose registers that must be loaded with
 *  descriptors for the GDT and IDT. Both registers use the same 
 *  descriptor structure, consisting of a pointer to the start of the
 *  table, and the size of the table.
 */
struct table_descriptor
{
    uint16_t size;
    uint32_t base_address;
} 
__attribute__ ((packed));

/**********************************************************/

/**
 *  Each entry of the GDT has this format. Logically, there are four
 *  fields of this struct: base, limit, flags and access. Base is a 32 bit
 *  physical address of the base of the segment; limit is a 20 bit value
 *  that describes the length of the segment (in units of either 1 byte or
 *  4k bytes depending on the granularity flag bit). Flags and access are
 *  bitmaps with various properties.
 */
struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access_bits;
    uint8_t flags_and_limit_high;
    uint8_t base_high;
}
__attribute__ ((packed));

/** macros for setting fields of the access bitmap */
/** present bit must be set for all valid descriptors */
#define GDT_PRESENT(x)          ((x) << 7)

/** priv bits, defines the ring level of this segment. 0 means kernel
 *  mode, 3 means userland/lowest privilege */
#define GDT_RING_LEVEL(x)       (((x) & 0x03) << 5)

/** if executable=1, the contents of this segment can be executed */
#define GDT_EXECUTABLE(x)       ((x) << 3)

/** read/write permission bit. Setting this bit enables write permission
 *  for a data segment, or read permission for a code segment. Note that
 *  write access is never allowed for code segments, and read access is
 *  always allowed for data segments.
 */
#define GDT_READ_WRITE(x)       ((x) << 1)


/** macros for setting fields of the flags nibble */
/** granularity = 1: limit is in units of 4 kB */
#define GDT_GRANULARITY(x)      ((x) << 7)

/** 32 bit or 16 bit. size=1: 32 bit segment. */
#define GDT_SIZE(x)             ((x) << 6)


/**********************************************************/

/**
 *  Structure for each entry of the interrupt descriptor table. Similar to
 *  the GDT entries, the IDT entry layout is a bit convoluted, but 
 *  consists of three fields: a pointer to an interrupt handler, an 
 *  attributes bitmap and a segment descriptor for the segment to use to
 *  execute the handler.
 */
struct idt_entry
{
    uint16_t handler_low;
    uint16_t segment_selector;
    uint8_t unused;
    uint8_t attributes;
    uint16_t handler_high;
}
__attribute__ ((packed));

/** macros for setting the attribute bits of an IDT entry */
/** present bit can be turned off for interrupts that are not in use */
#define IDT_PRESENT             0x80

/** privilege required to invoke this interrupt */
#define IDT_RING_LEVEL(x)       (((x) & 0x03) << 5)

/** define whether this is a trap or task call */
#define INTERRUPT_GATE          0xE
#define TRAP_GATE               0xF
#define TASK_GATE               0x5


/**********************************************************/

void make_gdt_entry (struct gdt_entry *entry, uint32_t base, 
  uint32_t limit, uint8_t flags, uint8_t access_bits);

void make_idt_entry (struct idt_entry *entry, uint32_t handler_address,
  uint16_t segment_selector, uint8_t attributes);


#endif /** _DESCRIPTORS_H */

/** vim: set ts=4 sw=4 et : */
