/**
 *  Functions for printing strings and numbers onto the screen via the
 *  VGA.
 */

#ifndef _OUTPUT_H
#define _OUTPUT_H

#include <stdint.h>

/**********************************************************/

void print_string (const char *string);
void print_int_hex (int value);
void print_integer (int value);

/**********************************************************/

#endif // _OUTPUT_H

/** vim: set ts=4 sw=4 et : */
