/**
 *  Code for initialising a PS/2 controller to read from a keyboard, and
 *  functions for sending and receiving data.
 */

#include "stdint.h"
#include "utils.h"
#include "io.h"
#include "ps2.h"

/**********************************************************/

PRIVATE void disable_ports (void);
PRIVATE void set_config (void);

PRIVATE uint8_t read_config_register (void);
PRIVATE void write_config_register (uint8_t value);

/**********************************************************/

/**
 *  Initialise the PS/2 controller by disabling all devices and interrupts
 *  and then getting the controller to run its self tests. On return, both
 *  PS/2 ports and interrupts will be disabled, and translation of scan
 *  codes will be off.
 */
    PUBLIC void
ps2_initialise (void)
{
    disable_ports ();
    set_config ();
}

/**********************************************************/

/**
 *  Enable the keyboard interrupt from the PS/2 controller.
 */
    PUBLIC void
enable_keyboard_irq (void)
{
    uint8_t config = read_config_register ();
    config |= ENABLE_IRQ1;
    write_config_register (config);
}

/**********************************************************/

/**
 *  Read the next byte from the PS/2 data port, waiting for the status bit
 *  indicating that data is available to be set.
 */
    PUBLIC uint8_t
ps2_get (void)
{
    uint8_t status = inb (PS2_STATUS);

    while ((status & INPUT_AVAILABLE) == 0)
        status = inb (PS2_STATUS);

    return inb (PS2_DATA);
}

/**********************************************************/

/**
 *  Write data out to a PS/2 device. This function will wait until the
 *  status bit indicates that the hardware is ready for data to be sent.
 */
    PUBLIC void
ps2_send (port, data)
    uint16_t port;              // port to write to, 0x60 or 0x64
    uint8_t data;               // value to write.
{
    uint8_t status = inb (PS2_STATUS);

    while ((status & BUSY) != 0)
        status = inb (PS2_STATUS);

    outb (port, data);
}

/**********************************************************/

/**
 *  Disable both ports of the PS/2 controller, and clear any input from
 *  the devices that may be sitting in the buffer.
 */
    PRIVATE void
disable_ports (void)
{
    ps2_send (PS2_COMMAND, DISABLE_PORT_1);
    ps2_send (PS2_COMMAND, DISABLE_PORT_2);

    clear_buffer ();
}

/**********************************************************/

/**
 *  Disable both PS/2 interrupts by clearing the appropriate bits in the
 *  config register.
 */
    PRIVATE void
set_config (void)
{
    uint8_t config = read_config_register ();
    config &= ~ (ENABLE_IRQ1 | ENABLE_IRQ12 | TRANSLATION);
    write_config_register (config);
}

/**********************************************************/

/**
 *  Discard any input from devices that may be waiting in the buffer.
 */
    PUBLIC void
clear_buffer (void)
{
    uint8_t status = inb (PS2_STATUS);

    while ((status & INPUT_AVAILABLE) != 0)
    {
        status = inb (PS2_STATUS);
        inb (PS2_DATA);
    }
}

/**********************************************************/

/**
 *  Returns the contents of the PS/2 configuration register.
 */
    PRIVATE uint8_t
read_config_register (void)
{
    uint8_t contents;

    ps2_send (PS2_COMMAND, READ_CONFIG);
    contents = ps2_get ();

    return contents;
}

/**********************************************************/

/**
 *  Store the given value in the PS/2 config register. Note that since
 *  the register contains several bits that are undefined, we should not
 *  change them. The register should be read first, then some bits
 *  modified, and the modified value written with this function.
 */
    PRIVATE void
write_config_register (value)
    uint8_t value;              // new register contents to write.
{
    ps2_send (PS2_COMMAND, WRITE_CONFIG);
    ps2_send (PS2_DATA, value);
}

/**********************************************************/

/** vim: set ts=4 sw=4 et : */
