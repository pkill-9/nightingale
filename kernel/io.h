/**
 *  Functions for reading and writing on IO ports.
 *
 *  Intel CPUs have IO ports for communicating with many hardware devices,
 *  and special assembly instructions to read and write on IO ports. These
 *  functions are wrappers to these assembly instructions, making it
 *  simpler to communicate with hardware devices from C code.
 *
 *  These functions are implemented in assembly language.
 */

#ifndef _IO_H
#define _IO_H

#include "stdint.h"

/**********************************************************/

void outb (uint16_t port, uint8_t value);
uint8_t inb (uint16_t port);

void disable_interrupts (void);

/**********************************************************/


#endif /** _IO_H */

/** vim: set ts=4 sw=4 et : */
