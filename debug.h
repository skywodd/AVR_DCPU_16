/**
 * Serial debug utility for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
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
 * Drop debug message to UART port (Use DEBUG_STR(x) macro instead, don't call this function directly !)
 */
void serial_debug_str(PGM_P filename, uint16_t line_number, PGM_P message, PGM_P value);

/**
 * Drop debug message to UART port (Use DEBUG_DEC(x), DEBUG_HEX(x) or DEBUG_BIN(x) macro instead, don't call this function directly !)
 */
void serial_debug_val(PGM_P filename, uint16_t line_number, PGM_P message, uint16_t value, uint8_t mode);

#endif