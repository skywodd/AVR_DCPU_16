/**
 * Serial debug utility for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
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