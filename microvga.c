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
#include <avr/io.h>     // For GPIO
#include <stdint.h>     // For hardcoded int type
#include <util/delay.h> // For delay
#include "pinmap.h"     // For pin mapping
#include "uart.h"       // For UART
#include "ram.h"        // For RAM
#include "buzzer.h"     // For buzzer
#include "debug.h"      // For debug
#include "microvga.h"   // For prototypes

/* Prototypes */
static inline uint8_t OFFSET_TO_X(uint16_t offset); // Compute offset to X value
static inline uint8_t OFFSET_TO_Y(uint16_t offset); // Compute offset to Y value

/* Defines */
#define ESC '\033'    // Escape character

/* Display macro */
#define GET_FOREGROUND_COLOR(x) ((x & 0xF000) >> 12) // Get foreground color from data
#define GET_BACKGROUND_COLOR(x) ((x & 0x0F00) >> 8)  // Get background color from data
#define GET_BLINKING_BIT(x) (x & 0x80)               // Get blink mode bit from data
#define GET_ASCII_CHAR(x) (x & 0x7F)                 // Get ascii char from data

/* Color scheme */
static uint8_t color_scheme[8] = {
  0, 4, 2, 6, 1, 5, 3, 7
}; // Color scheme of "DCPU-16 studio" software

/* Global variables */
static volatile uint8_t keyboard_flag = 0;

/* Interrupts */
// MicroVGA keyboard keypress
// -> Take a look in uart.c

void microvga_setup(void) {

  // CS and CTS as output
  MICROVGA_DDR |= (1 << MICROVGA_CTS);
  MICROVGA_CS_DDR |= (1 << MICROVGA_CS);

  // RTS as input
  MICROVGA_DDR &= ~(1 << MICROVGA_RTS);
  
  // Disable by default
  microvga_disable();

}

void microvga_enable(void) {

  // Chip select to LOW
  MICROVGA_CS_PORT &= ~(1 << MICROVGA_CS);
  __builtin_avr_delay_cycles(100);
  
  // Allow UART transmission
  MICROVGA_PORT &= ~(1 << MICROVGA_CTS);
  __builtin_avr_delay_cycles(100);
  
  DEBUG_STR("MicroVGA", "enable");
  
}

void microvga_disable(void) {

  // Chip select to HIGH
  MICROVGA_CS_PORT |= (1 << MICROVGA_CS);
  __builtin_avr_delay_cycles(100);
  
  // Disallow UART transmission
  MICROVGA_PORT |= (1 << MICROVGA_CTS); 
  __builtin_avr_delay_cycles(100);
  
  DEBUG_STR("MicroVGA", "disable");
  
}

void microvga_hide_cursor(void) {

  uart_write(ESC); // Escape command
  uart_write('['); // Command mode
  uart_write('2'); //
  uart_write('5'); //
  uart_write('l'); // Hide cursor
  _delay_us(50);   // 
  
}

void microvga_show_cursor(void) {

  uart_write(ESC); // Escape command
  uart_write('['); // Command mode
  uart_write('2'); //
  uart_write('5'); //
  uart_write('h'); // Enable cursor
  _delay_us(50);   // 
  
}

void microvga_set_attribute(microvga_attribute attribute) {

  uart_write(ESC);             // Escape command
  uart_write('[');             // Command mode
  uart_write(attribute + '0'); // Attribute
  uart_write('m');             // Set mode
  _delay_us(50);   // 
  
}

void microvga_set_foreground_color(microvga_color color) {

  uart_write(ESC);         // Escape command
  uart_write('[');         // Command mode
  uart_write('3');         // Foregroud color
  uart_write(color + '0'); // Color
  uart_write('m');         // Set mode
  _delay_us(50);   // 
  
}

void microvga_set_background_color(microvga_color color) {

  uart_write(ESC);         // Escape command
  uart_write('[');         // Command mode
  uart_write('4');         // Background color
  uart_write(color + '0'); // Color
  uart_write('m');         // Set mode
  _delay_us(50);   // 
  
}

void microvga_clear_eol(void) {

  uart_write(ESC); // Escape command
  uart_write('['); // Command mode
  uart_write('K'); // Clear EOL
  _delay_us(50);   // 
  
}

void microvga_clear_screen(void) {

  uart_write(ESC); // Escape command
  uart_write('['); // Command mode
  uart_write('2'); // Clear screen
  uart_write('J'); //
  _delay_us(50);   // 
  
}

uint8_t OFFSET_TO_X(uint16_t offset) {

  return (uint8_t)(offset % 32); // Compute column index from offset
  
}

uint8_t OFFSET_TO_Y(uint16_t offset) {

  return (uint8_t)(offset / 32); // Compute row index from offset
  
}

void microvga_goto_cursor(uint8_t x, uint8_t y) {

  uart_write(ESC);            // Escape command
  uart_write('[');            // Command mode
  uart_write((y / 10) + '0'); // Row 
  uart_write((y % 10) + '0'); //
  uart_write(';');            // Separator
  uart_write((x / 10) + '0'); // Column
  uart_write((x % 10) + '0'); //
  uart_write('f');            // Goto XY
  _delay_us(50);              // 
  
}

void microvga_move_cursor(microvga_move move, uint8_t count) {

  uart_write(ESC);                // Escape command
  uart_write('[');                // Command mode
  uart_write((count / 10) + '0'); // Decimal count
  uart_write((count % 10) + '0'); //
  uart_write(move + 'A');         // Move cursor
  _delay_us(50);                  // 
  
}

void microvga_set_font(microvga_font font) {

  uart_write(ESC);        // Escape command
  uart_write('[');        // Command mode
  uart_write('1');        // Page font
  uart_write('0');        //
  uart_write(font + '0'); // page code
  uart_write('l');        // Select font page
  _delay_us(50);          // 
  
}

void screen_word_interpret(uint16_t offset, uint16_t data) {

  uint8_t color;                   // Temp buffer
  uint8_t x = OFFSET_TO_X(offset); // Compute column index from offset
  uint8_t y = OFFSET_TO_Y(offset); // Compute row index from offset
  
  if(!data) return;
  
  // Go to pixel (x, y)
  microvga_goto_cursor(x, y);
  
  DEBUG_STR("MicroVGA", "interpret");
  DEBUG_DEC("MicroVGA pos X", x);
  DEBUG_DEC("MicroVGA pos Y", y);
  
  DEBUG_DEC("MicroVGA blink", GET_BLINKING_BIT(data));
  
#ifdef BLINKING_BIT_SUPPORT
  // Set or unset blink mode
  if(GET_BLINKING_BIT(data))                             // If blink bit is set
    microvga_set_attribute(ATTRIBUTE_BLINK_MODE);        // Set blink mode
  else                                                   // Else
    microvga_set_attribute(ATTRIBUTE_NOBLINK_MODE);      // Unset blink mode 
#endif

#ifdef FOREGROUND_COLOR_SUPPORT
  // Set foreground color
  color = GET_FOREGROUND_COLOR(data);                    // Get foreground color
  DEBUG_DEC("MicroVGA foreground", color);
  if(color >= 8) {                                       // If color >= 8
    color -= 8;                                          // Apply color offset
    microvga_set_attribute(ATTRIBUTE_BRIGHT_FOREGROUND); // Set high density color
  } else {                                               // Else
    microvga_set_attribute(ATTRIBUTE_DIM_FOREGROUND);    // Set low density color
  }                                                      //
  microvga_set_background_color(color_scheme[color]);    // Set foreground color
#endif

#ifdef BACKGROUND_COLOR_SUPPORT
  // Set background color
  color = GET_BACKGROUND_COLOR(data);                    // Get background color
  DEBUG_DEC("MicroVGA background", color);
  if(color >= 8) {                                       // If color >= 8
    color -= 8;                                          // Apply color offset
    microvga_set_attribute(ATTRIBUTE_BRIGHT_BACKGROUND); // Set high density color
  } else {                                               // Else
    microvga_set_attribute(ATTRIBUTE_DIM_BACKGROUND);    // Set low density color
  }                                                      //
  microvga_set_background_color(color_scheme[color]);    // Set background color
#endif
  
  // Display character on screen
  uart_write(GET_ASCII_CHAR(data));
  _delay_us(50);
  
}

void keyboard_wait(void) {

  while(!keyboard_flag); // Wait for keyboard flag to be set
  keyboard_flag = 0;     // Reset keyboard flag it and return
  
}

void keyboard_flag_set(void) {

  keyboard_flag = 1; // Set keyboard flag
  
}

uint8_t microvga_ready(void) {

  return !(MICROVGA_PIN & (1 << MICROVGA_RTS));
  
}