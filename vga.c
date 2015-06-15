/**
 *  Functions relating to VGA display hardware.
 */

#include "vga.h"
#include "utils.h"

/** text mode VGA is by default 80 columns by 25 rows */
#define DISPLAY_ROWS            25
#define DISPLAY_COLUMNS         80

/** selectors for writing data to VGA hardware registers */
#define CURSOR_LOW_BYTE         0x0F
#define CURSOR_HIGH_BYTE        0x0E

/**********************************************************/

PRIVATE void forward_cursor (void);
PRIVATE void back_cursor (void);
PRIVATE void scroll (void);
PRIVATE void process_control_char (char c);

/**********************************************************/

/** current position of the cursor */
PRIVATE int cursor_row, cursor_column;
PRIVATE unsigned char text_colour;
PRIVATE volatile char *video_memory;

/**********************************************************/

/**
 *  Initialise the console parameters: cursor position text colour and
 *  video memory buffer.
 */
    PUBLIC void
vga_initialise (void)
{
    cursor_row = 0;
    cursor_column = 0;

    /** grey text on black background */
    text_colour = TEXT_COLOUR (GREY, BLACK);

    /** vga memory is mapped to physical address 0xB8000 */
    video_memory = (volatile char *) 0xB8000;

    /** set bit 0 of the miscelaneous output register. This ensures that
     *  other VGA registers are at the address we expect. */
    outb (0x3C2, inb (0x3CC) | 0x01);
}

/**********************************************************/

/**
 *  Set the cursor position.
 */
    PUBLIC void
set_cursor (row, column)
    int row;
    int column;
{
    if (row >= 0 && row < DISPLAY_ROWS)
        cursor_row = row;

    if (column >= 0 && column < DISPLAY_COLUMNS)
        cursor_column = column;
}

/**********************************************************/

/**
 *  Set the colour attribute for the next text to be printed.
 */
    PUBLIC void
set_colour (colour)
    unsigned char colour;
{
    text_colour = colour;
}

/**********************************************************/

/**
 *  Print a single char at the current cursor position with the current
 *  colour, and advance the cursor one space.
 *
 *  This function will also handle some non printable control characters,
 *  such as tab, carriage return and line feed.
 */
    PUBLIC void
print_char (character)
    char character;
{
    int index = (cursor_row * DISPLAY_COLUMNS + cursor_column) * 2;

    /** for printable chars, we will simply copy the char to the correct
     *  location in video memory and advance the cursor. If the char is
     *  not printable, we will not advance the cursor (that would not work
     *  with backspace for example). */
    if (isprintable (character))
    {
        video_memory [index] = character;
        video_memory [index + 1] = text_colour;
        forward_cursor ();
    }
    else
    {
        process_control_char (character);
    }
}

/**********************************************************/

/**
 *  Handles a selection of non printable characters.
 *
 *  \b: backspace. Erases the last character, and reverses the cursor.
 *  \t: advances the cursor to the next horizontal tab space.
 *  \n: move the cursor down to the next line.
 *  \r: move the cursor to the start of the line.
 *  \v: vertical tab. Advance the cursor down to the next tab line.
 *  \f: form feed. Clears the screen.
 */
    PRIVATE void
process_control_char (c)
    char c;
{
    switch (c)
    {
    case '\b':
        back_cursor ();
        print_char (' ');
        back_cursor ();
        break;

    case '\t':
        cursor_row += TAB_WIDTH - cursor_row % TAB_WIDTH;

        if (cursor_row >= DISPLAY_ROWS)
            cursor_row = DISPLAY_ROWS - 1;

        break;

    case '\v':
        cursor_column += TAB_WIDTH - cursor_column % TAB_WIDTH;

        if (cursor_column >= DISPLAY_COLUMNS)
            cursor_column = DISPLAY_COLUMNS - 1;

        break;

    case '\n':
        if (cursor_row < DISPLAY_ROWS)
        {
            cursor_row ++;
        }
        else
        {
            scroll ();
        }

        break;

    case '\r':
        cursor_column = 0;
        break;

    case '\f':
        clear_screen ();
        break;
    }
}

/**********************************************************/

/**
 *  Update the cursor position. This function should be called when some
 *  text has been printed, to place the cursor after the end of the text.
 *
 *  Note that it is best to avoid using this function too frequently, as
 *  it is a slow operation to update the cursor position.
 */
    PUBLIC void
print_done (void)
{
    unsigned short linear_position = cursor_row * DISPLAY_COLUMNS + 
        cursor_column;

    /** output the linear position in two bytes */
    outb (0x3D4, CURSOR_LOW_BYTE);
    outb (0x3D5, (unsigned char) linear_position & 0xFF);
    
    outb (0x3D4, CURSOR_HIGH_BYTE);
    outb (0x3D5, (unsigned char) linear_position >> 8 & 0xFF);
}

/**********************************************************/

/**
 *  Advance the cursor by one space. If the end of the current line is
 *  reached, this will move the cursor to the start of the next line.
 *
 *  If the cursor is at the end of the last line on the screen, then the
 *  screen will be scrolled forward, and the cursor will go back to the
 *  start of the last line on the screen.
 */
    PRIVATE void
forward_cursor (void)
{
    cursor_column += 1;

    if (cursor_column >= DISPLAY_COLUMNS)
    {
        cursor_row += 1;
        cursor_column = 0;
    }

    if (cursor_row >= DISPLAY_ROWS)
        scroll ();
}

/**********************************************************/

/**
 *  Move the cursor back by one space, eg if backspace was pressed. If the
 *  cursor is already at the start of the line, the cursor will be placed
 *  at the end of the previous line.
 *
 *  Note that this function does not erase characters, it *just* moves the
 *  cursor.
 */
    PRIVATE void
cursor_back (void)
{
    if (cursor_column == 0)
    {
        if (cursor_row == 0)
            return;

        cursor_column = DISPLAY_COLUMNS - 1;
        cursor_row -= 1;
    }
    else
    {
        cursor_column -= 1;
    }
}

/**********************************************************/

/**
 *  Scroll the contents of the display up by one line. The top line of
 *  the screen will not be preserved by this operation.
 *
 *  Scrolling is done by copying the memory contents of each line to the
 *  memory of the previous line, with the exception of the first line,
 *  which gets overwritten.
 */
    PRIVATE void
scroll (void)
{
    for (int row = 1; row < DISPLAY_ROWS; row ++)
    {
        memcopy (video_memory + (row + 1) * 2, video_memory + row * 2,
          DISPLAY_COLUMNS * 2);
    }

    /** now clear the contents of the last line on the screen */
    for (int column = 0; column < DISPLAY_COLUMNS; column ++)
        video_memory [column * 2] = ' ';
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
