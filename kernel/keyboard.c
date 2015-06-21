/**
 *  Basic kernel mode PS/2 keyboard driver.
 *
 *  When a key is pressed, the keyboard hardware triggers IRQ1, and the
 *  handler will respond by reading in the scancode for the key that was
 *  pressed or released. That scancode is converted into a keycode, and
 *  then a key handler is called, as configured in the keycode mapping
 *  table. Some keys only need a handler for the press event, such as
 *  letter keys, whereas others need to handle both press and release,
 *  such as CTRL or SHIFT keys.
 */

#include "keycodes.h"
#include "scancodes.h"
#include "utils.h"

/**********************************************************/

/**
 *  This struct defines key handlers. When a key is pressed, the keycode
 *  is used as an index into an array of these handler structs, and gives
 *  a function to be called to process the key press/release. A NULL
 *  function pointer means that there is no handler assigned to that
 *  event, eg no handler for release of normal letter keys.
 */
typedef struct
{
    void (*onpress) (keycode_t key);
    void (*onrelease) (keycode_t key);
}
keyhandler_t;

/**********************************************************/

PRIVATE void alphabet_key_pressed (keycode_t key);
PRIVATE void number_key_pressed (keycode_t key);
PRIVATE void numpad_key_pressed (keycode_t key);
PRIVATE void punctuation_key_pressed (keycode_t key);
PRIVATE void lock_key_pressed (keycode_t key);
PRIVATE void modifier_pressed (keycode_t key);
PRIVATE void modifier_released (keycode_t key);

/**********************************************************/

PRIVATE bool shift_pressed, control_pressed, alt_pressed;
PRIVATE bool caps_lock, num_lock, scroll_lock;
PRIVATE keyhandler_t key_event_handlers [NUMKEYCODES];

/**********************************************************/

/**
 *  Handles alphabet keys.
 */
    PRIVATE void
alphabet_key_pressed (key)
    keycode_t key;
{
    const char letters [] = "qwertyuiopasdfghjklzxcvbnm";
    const char shifted [] = "QWERTYUIOPASDFGHJKLZXCVBNM";

    if (shift_pressed ^ caps_lock) 
    {
        character = shifted [key - ALPHABET_BASE];
        return;
    }

    character = letters [key - ALPHABET_BASE];

    if (control_pressed)
        character -= CONTROL_SHIFT;
}

/**********************************************************/

/**
 *  If caps lock, num lock or scroll lock is pressed, we need to invert
 *  the state of the lock.
 */
    PRIVATE void
lock_key_pressed (key)
    keycode_t key;
{
    if (key == CAPS_LOCK)
        caps_lock = caps_lock? false : true;

    if (key == NUM_LOCK)
        num_lock = num_lock? false : true;

    if (key == SCROLL_LOCK)
        scroll_lock = scroll_lock? false : true;

    set_keyboard_leds ();
}

/**********************************************************/

/**
 *  Handles modifier keys, CTRL, ALT and SHIFT. Upon a keypress, this
 *  function will switch on the flag to indicate that one of the modifiers
 *  has been pressed.
 */
    PRIVATE void
modifier_pressed (key)
    keycode_t key;
{
    if (key == LEFT_SHIFT || key == RIGHT_SHIFT)
        shift_pressed = true;

    if (key == LEFT_CTRL || key == RIGHT_CTRL)
        control_pressed = true;

    if (key == LEFT_ALT || key == RIGHT_ALT)
        alt_pressed = true;
}

/**********************************************************/

/**
 *  Switches of the appropriate flag when a modifier key is released.
 */
    PRIVATE void
modifier_released (key)
    keycode_t key;
{
    if (key == LEFT_SHIFT || key == RIGHT_SHIFT)
        shift_pressed = false;

    if (key == LEFT_CTRL || key == RIGHT_CTRL)
        control_pressed = false;

    if (key == LEFT_ALT || key == RIGHT_ALT)
        alt_pressed = false;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
