/**
 *  Functions for printing output onto the screen via the VGA
 */

#include "output.h"
#include "vga.h"
#include "utils.h"

/**********************************************************/

/**
 *  Function to print an integer in human readable form (base 10).
 *
 *  This uses a recursive algorithm to ensure that we print the digits in
 *  the correct order (most significant first) without needing some kind
 *  of a buffer. For each recursive call, the value is divided by 10, and
 *  then the digits are printed as we come back out from the stack of
 *  calls. Since the bottom of the stack will have the most significant
 *  digit, this means that digits will be printed in order of most
 *  significant to least.
 */
    PUBLIC void
print_integer (value)
    int value;                  // integer to be printed.
{
    // if the value is negative, we will print a - sign first.
    if (value < 0)
    {
        print_char ('-');
        value = ~value + 1; // two's complement hack for value *= -1.
    }

    if (value / 10 != 0)
        print_integer (value / 10);

    print_char ('0' + value % 10);
}

/**********************************************************/

/**
 *  Function for printing a 32 bit integer in hexadecimal form. This 
 *  function will also print the leading 0x prefix.
 */
    PUBLIC void
print_int_hex (value)
    int value;                  // value to be printed.
{
    const char *alphabet = "0123456789ABCDEF";
    int mask;
    int nibble_index = 32 - 4;
    int nibble;

    print_string ("0x");

    for (mask = 0xF0000000; mask != 0; mask >>= 4)
    {
        // mask out the next 4 bit part of the int, starting with the
        // most significant part.
        nibble = (value & mask) >> nibble_index;
        print_char (alphabet [nibble]);

        nibble_index -= 4;
    }

    print_done ();
}

/**********************************************************/

/**
 *  Prints a string. This is pretty standard, although probably could be
 *  optimised in assembly language.
 */
    PUBLIC void
print_string (string)
    const char *string;         // string to be printed.
{
    while (*string != '\0')
        print_char (*string ++);

    print_done ();
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
