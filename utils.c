/**
 *  C util functions.
 */

#include "utils.h"

/**********************************************************/

/**
 *  Check if the given character falls within the range of ASCII values
 *  that correspond to printed characters, including space. Return value
 *  is true for printed characters, false otherwise.
 *
 *  Note that tab and newline characters are regarded as control 
 *  characters by this function, and will evaluate as false.
 */
    PUBLIC bool
isprintable (character)
    char character;
{
    /** space has the smallest ASCII value of printable chars (0x20), and
     *  tilde ~ has the biggest value (0x7E). */
    if (character > ' ' && character < '~')
        return true;

    return false;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
