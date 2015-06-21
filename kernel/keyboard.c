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

PRIVATE void initialise_keyboard (void);
PRIVATE void init_keymap (void);
PRIVATE void clear_keyhandlers (void);

PRIVATE void keypressed (const char *scancode, const keycode_t *map);
PRIVATE void keyreleased (const char *scancode, const keycode_t *map);

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

PRIVATE keycode_t scancode_map [NUM_SCANCODES] = {0};
PRIVATE keycode_t alt_scancode_map [NUM_SCANCODES] = {0};


/**********************************************************/

/**
 *  Take a scancode and call the corresponding key handler function.
 *
 *  If the scancode corresponds to a key release event (0xF0 prefix) then
 *  this function will call the keyreleased function to handle it.
 */
    PRIVATE void
keypressed (scancode, map)
    const char *scancode;       // scancode from the keyboard.
    const keycode_t *map;       // scancode/keycode mapping to use.
{
    keycode_t keycode;

    // check the first byte of the scancode for prefixes that indicate to
    // use the extended keymap, or that it is a key release.
    switch (*scancode)
    {
    case 0xE0:
        keypressed (scancode + 1, alt_scancode_map);
        break;

    case 0xF0:
        keyreleased (scancode + 1, map);
        break;

    default:
        keycode = map [(uint8_t) *scancode];

        if (key_event_handlers [keycode].onpress != NULL)
            key_event_handlers [keycode].onpress (keycode);
    }
}

/**********************************************************/

/**
 *  Call the key release function for the given key. Key release scancodes
 *  have the same value as when the key is pressed, but with an 0xF0
 *  prefix byte before the scancode, which will be picked up by the 
 *  keypressed function.
 */
    PRIVATE void
keyreleased (scancode, map)
    const char *scancode;       // scancode without the prefix.
    const keycode_t *map;       // scancode-keycode mapping to use.
{
    keycode_t keycode;

    // XXX: extended scancode map prefix comes before the key release
    // prefix, so we should never see an 0xE0 prefix here, right?
    switch (*scancode)
    {
    case 0xE0:
        keyreleased (scancode + 1, alt_scancode_map);
        break;

    default:
        keycode = map [(uint8_t) *scancode];

        if (key_event_handlers [keycode].onrelease != NULL)
            key_event_handlers [keycode].onrelease (keycode);
    }
}

/**********************************************************/

/**
 *  Create a mapping from scancode to keycode, with the scancode as an
 *  index to allow O(1) lookup times. Note that scancodes.h defines the
 *  association from scancode to keycode, but to use that directly would
 *  require us to do a search over the association array, which is not
 *  ideal.
 */
    PRIVATE void
init_keymap (void)
{
    // there are two separate mappings, one for "regular" keys, and 
    // another for keys whose scancodes have the 0xE0 prefix.
    for (int i = 0; i < NUM_REGULAR_KEYS; i ++)
    {
        scancode_map [regular_keys [i].scancode] = 
            regular_keys [i].keycode;
    }

    for (int i = 0; i < NUM_EXTENDED_KEYS; i ++)
    {
        alt_scancode_map [extended_keys [i].scancode] =
            extended_keys [i].keycode;
    }
}

/**********************************************************/

/**
 *  Step through the array of key handlers and set the press and release
 *  function pointers to NULL, just in case there is some garbage in the
 *  memory.
 */
    PRIVATE void
clear_keyhandlers (void)
{
    for (int i = 0; i < NUMKEYCODES; i ++)
    {
        key_event_handlers [i].onpress = NULL;
        key_event_handlers [i].onrelease = NULL;
    }
}

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
