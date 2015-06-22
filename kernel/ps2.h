/**
 *  Constants and functions for working with an 8042 PS/2 controller chip.
 */

#ifndef _PS2_H
#define _PS2_H

/**********************************************************/

/** ports used by the 8042 controller. Note that port 0x64 links to two
 *  different registers on the 8042 chip; reads from port 0x64 come from
 *  the status register, but writes go to the command register.
 */
#define PS2_STATUS              0x64
#define PS2_COMMAND             0x64
#define PS2_DATA                0x60

/** we are only interested in two bits of the status register, which tell
 *  us when we can read or write to the data register.
 */
#define INPUT_AVAILABLE         0x01
#define BUSY                    0x02

/** Commands used by the 8042 chip. We are only interested in the commands
 *  for disabling and enabling the PS/2 ports, and for reading and writing
 *  the config byte.
 */
#define DISABLE_PORT_1          0xAD
#define DISABLE_PORT_2          0xA7
#define ENABLE_PORT_1           0xAE
#define ENABLE_PORT_2           0xA8
#define READ_CONFIG             0x20
#define WRITE_CONFIG            0x60

/** bits in the configuration byte. These control whether the PS/2 
 *  controller triggers the keyboard and mouse interrupts (IRQ1 and IRQ12)
 *  and also whether the controller translates scancodes to set 1.
 */
#define ENABLE_IRQ1             0x01
#define ENABLE_IRQ12            0x02
#define TRANSLATION             0x40


/**********************************************************/


#endif /** _PS2_H */

/** vim: set ts=4 sw=4 et : */
