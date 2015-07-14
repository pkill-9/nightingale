/**
 *  Main function for nightingale.
 */

#include "output.h"
#include "vga.h"
#include "keyboard.h"
#include "interrupts.h"
#include "ps2.h"
#include "utils.h"

/**********************************************************/

/**
 *  This function sets up the environment for the kernel to run. Most of
 *  the actual work is done in other initialise functions which are called
 *  here.
 */
    PUBLIC void
nightingale_initialise (void)
{
    vga_initialise ();
    pic_initialise ();
    ps2_initialise ();
    initialise_keyboard ();
}

/**********************************************************/

    PUBLIC void
nightingale_main (void)
{
    print_string ("It Works.\n");
    print_string ("Another line.\n");
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
