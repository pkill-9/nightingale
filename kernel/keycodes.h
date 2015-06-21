/**
 *  Definitions of keycodes for nightingale.
 *
 *  When a key is pressed, we will obtain a scancode from the hardware.
 *  Scancodes can be several bytes long, however, so it is not practical
 *  to use these as an index for a lookup table to handle the keypress.
 *  To get around this, scancodes are converted to keycodes, which are
 *  always a 2 byte long number. This keycode is then used as an index
 *  into a table of handler functions, which respond to key presses or
 *  releases by printing characters on the screen, changing CTRL or SHIFT
 *  key status, or setting NUM LOCK or CAPS LOCK leds.
 */

#ifndef _KEYCODES_H
#define _KEYCODES_H

#define ESC                 1

/** function keys */
#define F1                  2
#define F2                  3
#define F3                  4
#define F4                  5
#define F5                  6
#define F6                  7
#define F7                  8
#define F8                  9
#define F9                  10
#define F10                 11
#define F11                 12
#define F12                 13

/** punctuation keys */
#define KEY_TILDE           14
#define KEY_MINUS           15
#define KEY_EQUALS          16
#define KEY_SQUARE_OPEN     17
#define KEY_SQUARE_CLOSE    18
#define KEY_BACK_SLASH      19
#define KEY_SEMICOLON       20
#define KEY_APOSTROPHE      21
#define KEY_COMMA           22
#define KEY_PERIOD          23
#define KEY_FORWARD_SLASH   24

/** keyboard number keys (not the numpad on the right) */
#define KEY_1               25
#define KEY_2               26
#define KEY_3               27
#define KEY_4               28
#define KEY_5               29
#define KEY_6               30
#define KEY_7               31
#define KEY_8               32
#define KEY_9               33
#define KEY_0               34

/** letter keys */
#define KEY_Q               35
#define KEY_W               36
#define KEY_E               37
#define KEY_R               38
#define KEY_T               39
#define KEY_Y               40
#define KEY_U               41
#define KEY_I               42
#define KEY_O               43
#define KEY_P               44
#define KEY_A               45
#define KEY_S               46
#define KEY_D               47
#define KEY_F               48
#define KEY_G               49
#define KEY_H               50
#define KEY_J               51
#define KEY_K               52
#define KEY_L               53
#define KEY_Z               54
#define KEY_X               55
#define KEY_C               56
#define KEY_V               57
#define KEY_B               58
#define KEY_N               59
#define KEY_M               60

/** lock keys */
#define CAPS_LOCK           61
#define NUM_LOCK            62
#define SCROLL_LOCK         63

/** modifier keys */
#define LEFT_SHIFT          64
#define LEFT_CTRL           65
#define LEFT_ALT            66
#define RIGHT_SHIFT         67
#define RIGHT_CTRL          68
#define RIGHT_ALT           69

#define SPACE               70
#define BACKSPACE           71
#define TAB                 72

/** home and end group and arrow keys */
#define INSERT              80
#define HOME                81
#define PAGE_UP             82
#define DELETE              83
#define END                 84
#define PAGE_DOWN           85
#define ARROW_UP            86
#define ARROW_LEFT          87
#define ARROW_DOWN          88
#define ARROW_RIGHT         89

#define NUM_0               90
#define NUM_7               91
#define NUM_9               92
#define NUM_POINT           93
#define NUM_1               94
#define NUM_3               95
#define NUM_8               96
#define NUM_4               97
#define NUM_2               98
#define NUM_6               99

#define NUM_DIVIDE          73
#define NUM_MULTIPLY        74
#define NUM_MINUS           75
#define NUM_PLUS            76
#define NUM_ENTER           77

#define KEY_ENTER           78

#define PRINT_SCREEN        100
#define PAUSE               101


#endif /** _KEYCODES_H */

/** vim: set ts=4 sw=4 et : */
