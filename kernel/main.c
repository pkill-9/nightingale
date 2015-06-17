/**
 *  Main function for nightingale.
 */

#include "output.h"
#include "vga.h"
#include "utils.h"

/**********************************************************/

    PUBLIC void
nightingale_main (void)
{
    vga_initialise ();
    print_string ("It Works.\n");
    print_string ("Another line.\n");
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
