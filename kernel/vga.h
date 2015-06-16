/**
 *  Functions relating to VGA display hardware.
 */

#ifndef _VGA_H
#define _VGA_H

void vga_initialise (void);
void set_cursor (int row, int column);
void set_colour (unsigned char colour);
void print_char (char ch);
void print_done (void);


#endif /** _VGA_H */

/** vim: set ts=4 sw=4 et : */
