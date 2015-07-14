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

#include "keyboard.h"
#include "keycodes.h"
#include "scancodes.h"
#include "ps2.h"
#include "interrupts.h"
#include "stdint.h"
#include "utils.h"


/**********************************************************/

// IO port used by a PS/2 keyboard. Note that this is the same port as
// the controller's data port.
#define KEYBOARD                0x60

// keyboard commands.
#define SET_LEDS                0xED


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

PRIVATE void keyboard_handler (struct irq_hook *hook);
PRIVATE void init_keymap (void);
PRIVATE void clear_keyhandlers (void);

PRIVATE void keypressed (const keycode_t *map);
PRIVATE void keyreleased (const keycode_t *map);

PRIVATE void alphabet_key_pressed (keycode_t key);
PRIVATE void number_key_pressed (keycode_t key);
PRIVATE void numpad_key_pressed (keycode_t key);
PRIVATE void punctuation_key_pressed (keycode_t key);
PRIVATE void lock_key_pressed (keycode_t key);
PRIVATE void modifier_pressed (keycode_t key);
PRIVATE void modifier_released (keycode_t key);

PRIVATE void set_qwerty_handlers (void);
PRIVATE void set_numkey_handlers (void);
PRIVATE void set_punctuation_handlers (void);
PRIVATE void set_numpad_handlers (void);
PRIVATE void set_modifier_handlers (void);
PRIVATE void set_lock_handlers (void);


/**********************************************************/

/**
 *  States of the modifier keys and locks. These need to be in global
 *  variables, since they affect how other keypresses are interpretted.
 */
PRIVATE bool shift_pressed, control_pressed, alt_pressed;
PRIVATE bool caps_lock, num_lock, scroll_lock;

/**
 *  Array of functions to call on a key press or release; one for each
 *  keycode.
 */
PRIVATE keyhandler_t key_event_handlers [NUM_KEYCODES];

PRIVATE keycode_t scancode_map [256] = {0};
PRIVATE keycode_t alt_scancode_map [256] = {0};

PRIVATE irq_hook_t keyboard_hook;

PRIVATE char character;


/**********************************************************/

/**
 *  Initialise the kernel mode keyboard driver. This involves setting up
 *  an interrupt handler, and also setting up the data structures for
 *  translating key events into characters.
 *
 *  Note that interrupts and the PS/2 controller must be initialised 
 *  before this function is called.
 */
    PUBLIC void
initialise_keyboard (void)
{
    add_handler (&keyboard_hook, 2, keyboard_handler);
    enable_irq (2);
    ps2_enable_keyboard_irq ();

    init_keymap ();
    clear_keyhandlers ();

    set_qwerty_handlers ();
    set_numkey_handlers ();
    set_punctuation_handlers ();
    set_numpad_handlers ();
    set_modifier_handlers ();
    set_lock_handlers ();
}

/**********************************************************/

/**
 *  Keyboard IRQ handler. When a key is pressed, the keyboard triggers an
 *  interrupt, which results in this function being called. Here we will
 *  fetch the scancode from the keyboard and translate that into a 
 *  keypress or release.
 */
    PRIVATE void
keyboard_handler (hook)
    struct irq_hook *hook;      // this will be the keyboard hook
{
    keypressed (scancode_map);
    clear_buffer ();
}

/**********************************************************/

/**
 *  Take a scancode and call the corresponding key handler function.
 *
 *  If the scancode corresponds to a key release event (0xF0 prefix) then
 *  this function will call the keyreleased function to handle it.
 */
    PRIVATE void
keypressed (map)
    const keycode_t *map;       // scancode/keycode mapping to use.
{
    keycode_t keycode;
    uint8_t scancode = ps2_get ();

    // check the first byte of the scancode for prefixes that indicate to
    // use the extended keymap, or that it is a key release.
    switch (scancode)
    {
    case 0xE0:
        keypressed (alt_scancode_map);
        break;

    case 0xF0:
        keyreleased (map);
        break;

    default:
        keycode = map [scancode];

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
keyreleased (map)
    const keycode_t *map;       // scancode-keycode mapping to use.
{
    keycode_t keycode;
    uint8_t scancode = ps2_get ();

    // XXX: extended scancode map prefix comes before the key release
    // prefix, so we should never see an 0xE0 prefix here, right?
    switch (scancode)
    {
    case 0xE0:
        keyreleased (alt_scancode_map);
        break;

    default:
        keycode = map [scancode];

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
    for (int i = 0; i < NUM_KEYCODES; i ++)
    {
        key_event_handlers [i].onpress = NULL;
        key_event_handlers [i].onrelease = NULL;
    }
}

/**********************************************************/

/**
 *  Turn the CAPS LOCK and NUM LOCK leds on or off.
 */
    PRIVATE void
set_keyboard_leds (void)
{
    uint8_t ledstates = (scroll_lock | num_lock << 1 | caps_lock << 2);

    ps2_send (KEYBOARD, SET_LEDS);
    ps2_send (KEYBOARD, ledstates);
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

    if ((shift_pressed ^ caps_lock) && !control_pressed) 
    {
        character = shifted [key - FIRST_QWERTY];
        return;
    }

    character = letters [key - FIRST_QWERTY];

    if (control_pressed)
        character -= 'a' - 1;
}

/**********************************************************/

/**
 *  Main number keys, above the qwerty keys.
 */
    PRIVATE void
number_key_pressed (key)
    keycode_t key;
{
    const char normal [] = "1234567890";
    const char shifted [] = "!@#$%^&*()";

    if (shift_pressed)
    {
        character = shifted [key - FIRST_NUM_KEY];
        return;
    }

    character = normal [key - FIRST_NUM_KEY];
}

/**********************************************************/

/**
 *  Handles number pad keys. These keys will print a number only if num
 *  lock is on, otherwise they map to the home/end/arrow keys group.
 */
    PRIVATE void
numpad_key_pressed (key)
    keycode_t key;
{
    const char normal [] = "079.1384265/*+-\n";

    if (num_lock)
    {
        character = normal [key - FIRST_NUMPAD];
    }
}

/**********************************************************/

/**
 *  Handles punctuation keys.
 */
    PRIVATE void
punctuation_key_pressed (key)
    keycode_t key;
{
    const char normal [] = "`-=[]\\;',./";
    const char shifted [] = "~_+{}|:\"<>?";

    if (shift_pressed)
    {
        character = shifted [key - FIRST_PUNCTUATION];
        return;
    }

    character = normal [key - FIRST_PUNCTUATION];
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

/**
 *  These functions are used to store function pointers in the 
 *  key_event_handlers table so that the correct handler function gets
 *  called when a key is pressed or released. This is going to get pretty
 *  tedious, which is why it is at the end of the file.
 */
    PRIVATE void
set_qwerty_handlers (void)
{
    // alphabet keys only need a key press handler.
    for (int i = FIRST_QWERTY; i <= LAST_QWERTY; i ++)
        key_event_handlers [i].onpress = &alphabet_key_pressed;
}

/**********************************************************/

    PRIVATE void
set_numkey_handlers (void)
{
    for (int i = FIRST_NUM_KEY; i <= LAST_NUM_KEY; i ++)
        key_event_handlers [i].onpress = &number_key_pressed;
}

/**********************************************************/

    PRIVATE void
set_punctuation_handlers (void)
{
    for (int i = FIRST_PUNCTUATION; i <= LAST_PUNCTUATION; i ++)
        key_event_handlers [i].onpress = &punctuation_key_pressed;
}

/**********************************************************/

    PRIVATE void
set_numpad_handlers (void)
{
    for (int i = FIRST_NUMPAD; i <= LAST_NUMPAD; i ++)
        key_event_handlers [i].onpress = &numpad_key_pressed;
}

/**********************************************************/

    PRIVATE void
set_modifier_handlers (void)
{
    // modifier keys need to handle press and release.
    for (int i = FIRST_MODIFIER; i <= LAST_MODIFIER; i ++)
    {
        key_event_handlers [i].onpress = &modifier_pressed;
        key_event_handlers [i].onrelease = &modifier_released;
    }
}

/**********************************************************/

    PRIVATE void
set_lock_handlers (void)
{
    key_event_handlers [CAPS_LOCK].onpress = &lock_key_pressed;
    key_event_handlers [NUM_LOCK].onpress = &lock_key_pressed;
    key_event_handlers [SCROLL_LOCK].onpress = &lock_key_pressed;
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
