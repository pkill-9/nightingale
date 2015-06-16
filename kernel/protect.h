/**
 *  Declares the actual GDT and IDT structures, and some functions to
 *  set them up.
 */

#ifndef _PROTECT_H
#define _PROTECT_H

#include "descriptors.h"

#define NUM_GDT_ENTRIES         3
#define NUM_IDT_ENTRIES         256

struct table_descriptor gdtr;
struct table_descriptor idtr;

struct gdt_entry gdt [NUM_GDT_ENTRIES];
struct idt_entry idt [NUM_IDT_ENTRIES];


void initialise_tables (void);


#endif /** _PROTECT_H */

/** vim: set ts=4 sw=4 et : */
