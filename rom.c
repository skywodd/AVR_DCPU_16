/**
 * A25L010 Flash chip controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
/* Includes */
#include <avr/io.h> // For GPIO
#include <stdint.h> // For hardcoded int type
#include "pinmap.h" // For pin mapping
#include "spi.h"    // For SPI
#include "led.h"    // For led controller
#include "conf.h"   // For configuration
#include "debug.h"  // For debug
#include "rom.h"    // For prototypes

/* Global variables */
#ifdef ONBOARD_MEMORY
#include <avr/pgmspace.h>                  // For PROGMEM data
static uint16_t memory_rom[ONBOARD_ROM_SIZE] PROGMEM = { // Programm : Hello world
  /*0x8061, 0x8071, 0x7C11, 0xF100, 0x5801, 0x001D, 0x800C, 0x7DC1,
  0x002C, 0x7C0E, 0x00FF, 0x7DC1, 0x0013, 0x040A, 0x0171, 0x8000,
  0x8462, 0x8472, 0x91C1, 0x0011, 0x7C19, 0x00FF, 0xA017, 0x7C0E,
  0x01FF, 0x7C12, 0x0080, 0x8462, 0x91C1, 0x0170, 0x0048, 0x0065,
  0x006C, 0x006C, 0x006F, 0x0020, 0x02E1, 0x0077, 0x006F, 0x0072,
  0x006C, 0x0064, 0x0170, 0x0000, 0x81C1*/ // 45 opcodes
  // Programm : charmap + color test
  0x7C10, 0x0005, 0x85E2, 0x8000, 0x89C1, 0x8041, 0x8051, 0x8031,
  0x1011, 0x0C21, 0x7C10, 0x001B, 0x8452, 0x7C59, 0x00FF, 0x8432,
  0x7C3C, 0x0020, 0x7DC1, 0x0015, 0xA5C1, 0x8442, 0xC04C, 0x7DC1,
  0x001A, 0x9DC1, 0x61C1, 0x01A1, 0x05A1, 0x09A1, 0x1001, 0x9407,
  0x0C02, 0x9027, 0x081A, 0xA017, 0x141A, 0x0501, 0x8000, 0x6021,
  0x6011, 0x6001, 0x61C1 // 43 opcodes
};
#endif

void rom_setup(void) {

  // Chip select pins as output
  ROM_DDR |= (1 << ROM_SS);
  
  // Chip select pins to HIGH (ROM unselected)
  ROM_PORT |= (1 << ROM_SS);
  
}

// ROM
uint16_t rom_read(uint16_t address) {

  uint16_t data;    // ROM word readed
  
  led_rom_write(1); // Turn on activity led
  DEBUG_HEX("ROM read address", address);
  
#ifdef ONBOARD_MEMORY
  
  if(address >= ONBOARD_RAM_SIZE) { // If programm try to read an unavailable address
    DEBUG_STR("ROM", "read address out of space");
	led_rom_write(0);               // Turn off activity led
    return 0;                       // Return 0
  }
  
  data = pgm_read_word_near(memory_rom + address); // Read PROGMEM and return data
  
#else
                          
  uint32_t real_address = address * 2;             // Turn 2-words address into 1-byte address
  
  ROM_PORT &= ~(1 << ROM_SS);                      // Select ROM
  spi_transfer(0b00000011);                        // READ command
  spi_transfer((real_address & 0x00FF0000) >> 16); // MSB
  spi_transfer((real_address & 0x0000FF00) >> 8);  // 
  spi_transfer(real_address & 0x000000FF);         // LSB
  
  data = (uint16_t)spi_transfer(0xFF) << 8;        // Read data MSB
  data |= spi_transfer(0xFF);                      // Read data LSB
  
  ROM_PORT |= (1 << ROM_SS);                       // Unselect ROM
#endif
  
  led_rom_write(0);                                // Turn off activity led
  DEBUG_HEX("ROM read data", data);
  return data;                                     // Return ROM data
  
}