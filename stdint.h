/**
 *  Typedefs for integers of specified sizes.
 *
 *  This file ensures that nightingale is independant of the header files
 *  and C library of the host system.
 */

#ifndef _STDINT_H
#define _STDINT_H

/** we are targetting 32 bit systems, so we will assume a word size of 4
 *  bytes. */
typedef unsigned long long int  uint64_t;
typedef unsigned int            uint32_t;
typedef unsigned short int      uint16_t;
typedef unsigned char           uint8_t;


#endif /** _STDINT_H */

/** vim: set ts=4 sw=4 et : */
