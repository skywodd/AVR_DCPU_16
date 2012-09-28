/**
 * @file uart.h
 * @brief UART controller for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This controller handle all communications made using the UART port.\n
 * \n
 * Please report bug to <skywodd at gmail.com>
 *
 * @section licence_sec Licence
 *  This program is free software: you can redistribute it and/or modify\n
 *  it under the terms of the GNU General Public License as published by\n
 *  the Free Software Foundation, either version 3 of the License, or\n
 *  (at your option) any later version.\n
 * \n
 *  This program is distributed in the hope that it will be useful,\n
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of\n
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n
 *  GNU General Public License for more details.\n
 * \n
 *  You should have received a copy of the GNU General Public License\n
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.\n
 *
 * @section changelog_sec Changelog history
 * - 21/07/2012 : First version
 */

#ifndef _UART_H_
#define _UART_H_

#include <avr/pgmspace.h>

/**
 * Setup UART port and pins
 *
 * @param baudrate Bauds rate of UART port
 */
void uart_setup(uint32_t baudrate);

/**
 * Read a byte from UART port
 *
 * @warning Blocking function
 * @return Byte readed from UART port
 */
uint8_t uart_read(void);

/**
 * Write a byte to UART port
 *
 * @warning NON blocking function
 * @param data Byte to write to UART port
 */
void uart_write(uint8_t data);

/**
 * Write a string to UART port
 *
 * @warning NON blocking function
 * @param str String to write
 */
void uart_puts(const char *str);

/**
 * Write a PROGMEM string to UART port
 *
 * @warning NON blocking function
 * @param str PROGMEM string to write
 */
void uart_puts_PSTR(PGM_P str);

#endif