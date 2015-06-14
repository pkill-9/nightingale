/**
 *  Definitions of colour attributes for VGA text mode.
 *
 *  In text mode VGA, each character written to video memory also has an
 *  accompanying attribute byte, which defines the background and 
 *  foreground colours for the character, with the following format:
 *
 *  Bit 76543210
 *      ||||||||
 *      |||||^^^- foreground colour
 *      ||||^---- foreground colour bright bit
 *      |^^^----- background colour
 *      ^-------- background colour bright bit
 */

#ifndef _COLOURS_H
#define _COLOURS_H

/**
 *  Definitions of the basic colours. These can also be used as bright *,
 *  eg bright blue or bright green. Note that bright black is dark grey,
 *  and bright grey is white.
 */
#define BLACK               0
#define BLUE                1
#define GREEN               2
#define CYAN                3
#define RED                 4
#define MAGENTA             5
#define BROWN               6
#define GREY                7

/**
 *  A base colour can also be specified as bright by setting the most
 *  significant bit of the 4 bit colour nibble.
 */
#define BRIGHT(colour)      ((colour) | 8)

/**
 *  Construct a text attribute byte, given the background and foreground
 *  colours.
 */
#define TEXT_COLOUR(fg, bg) ((bg) << 4 | (fg))


#endif /** _COLOURS_H */

/** vim: set ts=4 sw=4 et : */
