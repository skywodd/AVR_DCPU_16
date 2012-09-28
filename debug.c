/*
 * See header file for details
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
 */
 
/* Includes */
#include <stdint.h> // For hardcoded types
#include <stdlib.h> // For itoa
#include "uart.h"   // For UART port
#include "debug.h"  // For prototypes

void serial_debug_str(PGM_P filename, uint16_t line_number, PGM_P message, PGM_P value) {
#ifdef SERIAL_DEBUG_SUPPORT

  char buf_line_number[17]; // sizeof(int) * 8 + 1 
  utoa(line_number, buf_line_number, 10);
  
  uart_puts_PSTR(PSTR("DEBUG: "));
  uart_puts_PSTR(filename);
  uart_write('@');
  uart_puts(buf_line_number);
  uart_puts_PSTR(PSTR(" - "));
  uart_puts_PSTR(message);
  uart_puts_PSTR(PSTR(" : "));
  uart_puts_PSTR(value);
  uart_write('\n');
  
#endif
}

void serial_debug_val(PGM_P filename, uint16_t line_number, PGM_P message, uint16_t value, uint8_t mode) {
#ifdef SERIAL_DEBUG_SUPPORT

  char buf_value[17]; // sizeof(int) * 8 + 1 
  char buf_line_number[17]; // sizeof(int) * 8 + 1 
  utoa(value, buf_value, mode);
  utoa(line_number, buf_line_number, 10);
  
  uart_puts_PSTR(PSTR("DEBUG: "));
  uart_puts_PSTR(filename);
  uart_write('@');
  uart_puts(buf_line_number);
  uart_puts_PSTR(PSTR(" - "));
  uart_puts_PSTR(message);
  uart_puts_PSTR(PSTR(" : "));
  uart_puts(buf_value);
  uart_write('\n');
  
#endif
}