/**
 * 23K256 RAM chip controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */

/* Includes */
#include <avr/io.h>   // For GPIO
#include <stdint.h>   // For hardcoded int type
#include "pinmap.h"   // For pin mapping
#include "conf.h"     // For configuration
#include "microvga.h" // For Screen
#include "spi.h"      // For SPI
#include "led.h"      // For led controller
#include "button.h"   // For button controller
#include "debug.h"    // For debug
#include "ram.h"      // For prototypes

/* Prototypes */
static uint16_t ram_select(uint16_t address);

/* Global variables */
#ifdef ONBOARD_MEMORY
static volatile uint16_t memory_ram[ONBOARD_RAM_SIZE]; // Onboard RAM
static volatile uint16_t memory_stack[ONBOARD_RAM_SIZE]; // Onboard stack
static volatile uint16_t keyboard_data = 0;            // Onboard keyboard data
#endif

void ram_setup(void) {

  // All chip select pins as output
  RAM_DDR |= (1 << RAM_BANK_A_SS) | (1 << RAM_BANK_B_SS) | (1 << RAM_BANK_C_SS) | (1 << RAM_BANK_D_SS);
  
  // All chip select pins to HIGH (RAM banks unselected)
  RAM_PORT |= (1 << RAM_BANK_A_SS) | (1 << RAM_BANK_B_SS) | (1 << RAM_BANK_C_SS) | (1 << RAM_BANK_D_SS);
  
}

uint16_t ram_select(uint16_t address) {

  uint32_t real_address = address * 2; // Turn 2-words address into 1-byte address
  
  if(address < 32768) {                // Bank A
    RAM_PORT &= ~(1 << RAM_BANK_A_SS); // Select RAM bank A
    
  } else if(address < 65536) {         // Bank B
    RAM_PORT &= ~(1 << RAM_BANK_B_SS); // Select RAM bank B
    real_address -= 32768;             // Apply offset
    
  } else if(address < 98304) {         // Bank C
    RAM_PORT &= ~(1 << RAM_BANK_C_SS); // Select RAM bank C
    real_address -= 65536;             // Apply offset
    
  } else {                             // Bank D
    RAM_PORT &= ~(1 << RAM_BANK_D_SS); // Select RAM bank D
    real_address -= 98304;             // Apply offset
  }
  
  return (uint16_t)real_address;       // Return real address with offset
  
}

uint16_t ram_read(uint16_t address) {

  uint16_t data;                    // RAM word readed

  led_ram_write(1);                 // Turn on activity led
  DEBUG_HEX("RAM read address", address);
  
#ifdef ONBOARD_MEMORY

  if(address == 0x9000) {           // If programm try to read the keyboard address
    led_ram_write(0);               // Turn off activity led
    return keyboard_data;           // Return keyboard data
  }

  if(address > (0xFFFF - ONBOARD_STACK_SIZE)) { // Stack space
    address = 0xFFFF - address;
    led_ram_write(0);               // Turn off activity led
	DEBUG_STR("RAM", "read stack address");
    return memory_stack[address];   // Return 0
  }
  
  if(address >= ONBOARD_RAM_SIZE) { // If programm try to read an unavailable address
    led_ram_write(0);               // Turn off activity led
	DEBUG_STR("RAM", "read address out of space");
    return 0;                       // Return 0
  }
  
  data = memory_ram[address];       // Return RAM data
  
#else

  uint16_t offset = ram_select(address); // Select required RAM bank
  
  spi_transfer(0b00000011);              // Send READ command
  spi_transfer(offset >> 8);             // Send address MSB
  spi_transfer(offset & 0x00FF);         // Send address LSB
  
  data = (uint16_t)spi_transfer(0xFF) << 8; // Read data MSB
  data |= spi_transfer(0xFF);                        // Read data LSB
  
  // Unselect all RAM banks
  RAM_PORT |= (1 << RAM_BANK_A_SS) | (1 << RAM_BANK_B_SS) | (1 << RAM_BANK_C_SS) | (1 << RAM_BANK_D_SS);
#endif

  DEBUG_HEX("RAM read data", data);
  led_ram_write(0); // Turn off activity led
  return data;      // Return RAM data
  
}

void ram_write(uint16_t address, uint16_t data) {

  led_ram_write(1); // Turn on activity led
  DEBUG_HEX("RAM write address", address);
  DEBUG_HEX("RAM write data", data);
  
  // Map $8000~$8200 to textmode screen
  if(address >= 0x8000 && address < 0x8200)        // If programm try to write to screen address
    screen_word_interpret(address - 0x8000, data); // Compute screen offset and call screen_write subroutine
  
#ifdef ONBOARD_MEMORY

  else {                                  // Else 
    if(address > (0xFFFF - ONBOARD_STACK_SIZE)) { // Stack space
      address = 0xFFFF - address;
      led_ram_write(0);                   // Turn off activity led
	  DEBUG_STR("RAM", "write stack address");
      memory_stack[address] = data;       
	  return;
    } else {
      if(address == 0x9000)               // If programm try to write to keyboard address
        keyboard_data = data;             // Store keyboard data locally
      else {                              // Else
        if(address >= ONBOARD_RAM_SIZE) { // If programm try to write an unavailable address
          led_ram_write(0);               // Turn off activity led
          button_set_state(0);            // Turn on error led and stop CPU
		  DEBUG_STR("RAM", "write address out of space");
		  return;                         // Fatal error ...
        } else                            // Else
          memory_ram[address] = data;     // Store data in local RAM space
      }
    }
  }
#else

  uint16_t offset = ram_select(address); // Select required RAM bank
  
  spi_transfer(0b00000010);      // Send WRITE command
  spi_transfer(offset >> 8);     // Send address MSB
  spi_transfer(offset & 0x00FF); // Send address LSB
  
  spi_transfer(data >> 8);       // Send data MSB
  spi_transfer(data & 0x00FF);   // Send data LSB
  
  // Unselect all RAM banks
  RAM_PORT |= (1 << RAM_BANK_A_SS) | (1 << RAM_BANK_B_SS) | (1 << RAM_BANK_C_SS) | (1 << RAM_BANK_D_SS); 
  
#endif

  led_ram_write(0); // Turn off activity led
  
}