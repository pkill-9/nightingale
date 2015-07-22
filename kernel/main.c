/**
 *  Main function for nightingale.
 */

#include "output.h"
#include "protect.h"
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
    initialise_tables ();
    vga_initialise ();
    pic_initialise ();
    ps2_initialise ();
    initialise_keyboard ();
}

/**********************************************************/

    PUBLIC void
nightingale_main (void)
{
    char ch;

    print_string ("Waiting for text input\n");

    while (true)
    {
        ch = read_char ();
        print_char (ch);
    }
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
