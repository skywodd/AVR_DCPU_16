/**
 * UART controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
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
 * Read a byte from UART port (blocked function)
 *
 * @return Byte readed from UART port
 */
uint8_t uart_read(void);

/**
 * Write a byte to UART port (non-blocked function)
 *
 * @param data Byte to write to UART port
 */
void uart_write(uint8_t data);

/**
 * Write a string to UART port (non-blocked function)
 *
 * @param str String to write
 */
void uart_puts(const char *str);

/**
 * Write a PROGMEM string to UART port (non-blocked function)
 *
 * @param str PROGMEM string to write
 */
void uart_puts_PSTR(PGM_P str);

#endif