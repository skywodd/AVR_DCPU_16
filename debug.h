/**
 * @file debug.h
 * @brief Serial debug utility for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This functions bundle implement some usefull debugging functions.\n
 * This bundle is not used for final release compilation but can be very usefull for other AVR based project.\n
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

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <avr/pgmspace.h>

#ifdef SERIAL_DEBUG_SUPPORT
#define DEBUG_STR(msg, val) serial_debug_str(PSTR(__FILE__), __LINE__, PSTR(msg), PSTR(val))
#define DEBUG_DEC(msg, val) serial_debug_val(PSTR(__FILE__), __LINE__, PSTR(msg), val, 10)
#define DEBUG_HEX(msg, val) serial_debug_val(PSTR(__FILE__), __LINE__, PSTR(msg), val, 16)
#define DEBUG_BIN(msg, val) serial_debug_val(PSTR(__FILE__), __LINE__, PSTR(msg), val, 2)
#else
#define DEBUG_STR(msg, val) {}
#define DEBUG_DEC(msg, val) {}
#define DEBUG_HEX(msg, val) {}
#define DEBUG_BIN(msg, val) {}
#endif

/**
 * Drop debug message (string) to UART port 
 *
 * @warning Use DEBUG_STR(x) macro instead, don't call this function directly !
 * @param filename Source code filename
 * @param line_number Line number in source code
 * @param message Debug message header
 * @param value Debug message value
 */
void serial_debug_str(PGM_P filename, uint16_t line_number, PGM_P message, PGM_P value);

/**
 * Drop debug message (numerical) to UART port
 *
 * @warning Use DEBUG_DEC(x), DEBUG_HEX(x) or DEBUG_BIN(x) macro instead, don't call this function directly !
 * @param filename Source code filename
 * @param line_number Line number in source code
 * @param message Debug message header
 * @param value Debug message value
 * @param mode Message value numerical base (2 = binary, 16 = hexa, 10 = decimal)
 */
void serial_debug_val(PGM_P filename, uint16_t line_number, PGM_P message, uint16_t value, uint8_t mode);

#endif