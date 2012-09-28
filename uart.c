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
#include <avr/io.h>        // For GPIO
#include <stdint.h>        // For hardcoded int type
#include <avr/interrupt.h> // For ISR
#include <avr/pgmspace.h>  // For PROGMEM
#ifdef UART_FLOW_CONTROL_SUPPORT
#include <util/delay.h>    // For delay
#endif
#include "pinmap.h"        // For pin mapping
#include "microvga.h"      // For MicroVGA controller
#include "ram.h"           // For RAM
#include "buzzer.h"        // For buzzer
#include "debug.h"         // For debug
#include "uart.h"          // For prototypes

/* Global variables */
static volatile uint8_t key_modifer_flag = 0;

/* Interrupts */
// UART receive byte event
ISR(USART_RX_vect) {
#ifdef KEYBOARD_INPUT_SUPPORT

  uint8_t c = uart_read(); // Read UART data (keyboard keypress)
  if(!c) {                 // If NULL char received
    key_modifer_flag = 1;  // Set key modifier flag
	DEBUG_STR("UART", "key modifier received");
    return;                // Return
  }
  
  if(ram_read(0x9000)) {    // Check keyboard RAM address
    DEBUG_STR("UART", "keypress received but ignored");
    buzzer_beep();          // If data exist beep buzzer
  } else {                  // Else
    if(key_modifer_flag) {  // If key modifier have been receive previously
	  DEBUG_STR("UART", "set keypress with modifier");
      ram_write(0x9000, 0x0100 | c);  // Store data to keyboard RAM address with key modifier flag
	  key_modifer_flag = 0; // Reset key modifier flag
	} else {                // Else
	  DEBUG_STR("UART", "set raw keypress");
	  ram_write(0x9000, c); // Store raw data to keyboard RAM address
	}
  }
    
  keyboard_flag_set();     // Set keyboard flag
  
#endif
}

void uart_setup(uint32_t baudrate) {
  
  // Compute baudrate
  baudrate = (F_CPU / 16 / baudrate) - 1;
  
  // Rx pin as input
  UART_DDR &= ~(1 << UART_RX);
  
  // Tx pin as output
  UART_DDR |= (1 << UART_TX);
  
  // Setup baudrate
  UBRR0H = (uint8_t)(baudrate >> 8);
  UBRR0L = (uint8_t)baudrate;
  DEBUG_DEC("UART UBRR0", baudrate);
  
  // Enable Rx & Tx
  UCSR0A = 0;
#ifdef KEYBOARD_INPUT_SUPPORT
  UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
#else
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
#endif
  
  // Set mode : 1 bit start, 8 bits data, 1 bit stop
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  
}

uint8_t uart_read(void) {

  while (!(UCSR0A & (1 << RXC0)));  // Wait for data to be received
  return UDR0;                      // Return received byte
  
}

void uart_write(uint8_t data) {
#ifdef UART_FLOW_CONTROL_SUPPORT
  __builtin_avr_delay_cycles(100);  // Wait for MicroVGA to handle communication
  while(!microvga_ready());         // Wait for MicroVGA to become ready
#endif

  while (!(UCSR0A & (1 << UDRE0))); // Wait for any previous write to finish
  UDR0 = data;                      // Send byte
  __builtin_avr_delay_cycles(100);  // Let time to UART to transfert data 
  
}

void uart_puts(const char *str) {

  while(*str != 0x00) { // While not end of string
    uart_write(*str++); // Put char
  }
  
}

void uart_puts_PSTR(PGM_P str) {

  uint8_t c;
  while((c = pgm_read_byte_near(str++))) {
    uart_write(c);
  }

}