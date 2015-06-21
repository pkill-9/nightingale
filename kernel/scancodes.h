/**
 *  Scancode to keycode mappings.
 *
 *  Scancode set 2 defines two categories of scancode, some "ordinary"
 *  ones, which are one bit long, and some others for special keys, such
 *  as multimedia controls, which have an 0xE0 prefix and are two bytes
 *  long.
 */

#ifndef _SCANCODES_H
#define _SCANCODES_H

#include "keycodes.h"

typedef struct
{
    uint16_t keycode;
    uint8_t scancode;
}
keymap_t;

keymap_t normal_keys [] = {
    {ESC, 0x76}
    {F1, 0x05}
    {F2, 0x06}
    {F3, 0x04}
    {F4, 0x0C}
    {F5, 0x03}
    {F6, 0x0B}
    {F7, 0x83}
    {F8, 0x0A}
    {F9, 0x01}
    {F10, 0x09}
    {F11, 0x78}
    {F12, 0x07}

    {KEY_TILDE, 0x0E}
    {KEY_1, 0x16}
    {KEY_2, 0x1E}
    {KEY_3, 0x26}
    {KEY_4, 0x25}
    {KEY_5, 0x2E}
    {KEY_6, 0x36}
    {KEY_7, 0x3D}
    {KEY_8, 0x3E}
    {KEY_9, 0x46}
    {KEY_0, 0x45}
    {KEY_MINUS, 0x4E}
    {KEY_EQUALS, 0x55}
    {BACKSPACE, 0x66}
    {TAB, 0x0D}

    {KEY_Q, 0x15}
    {KEY_W, 0x1D}
    {KEY_E, 0x24}
    {KEY_R, 0x2D}
    {KEY_T, 0x2C}
    {KEY_Y, 0x35}
    {KEY_U, 0x3C}
    {KEY_I, 0x43}
    {KEY_O, 0x44}
    {KEY_P, 0x4D}
    {KEY_A, 0x1C}
    {KEY_S, 0x1B}
    {KEY_D, 0x23}
    {KEY_F, 0x2B}
    {KEY_G, 0x34}
    {KEY_H, 0x33}
    {KEY_J, 0x3B}
    {KEY_K, 0x42}
    {KEY_L, 0x4B}
    {KEY_Z, 0x1A}
    {KEY_X, 0x22}
    {KEY_C, 0x21}
    {KEY_V, 0x2A}
    {KEY_B, 0x32}
    {KEY_N, 0x31}
    {KEY_M, 0x3A}

    {CAPS_LOCK, 0x58}
    {NUM_LOCK, 0x77}
    {SCROLL_LOCK, 0x7E}

    {LEFT_SHIFT, 0x12}
    {LEFT_CTRL, 0x14}
    {LEFT_ALT, 0x11}
    {RIGHT_SHIFT, 0x59}
}


#endif /** _SCANCODES_H */

/** vim: set ts=4 sw=4 et : */
