/**
 * Status led controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
/* Includes */
#include <avr/io.h> // For GPIO
#include <stdint.h> // For hardcoded int type
#include "pinmap.h" // For pin mapping
#include "led.h"    // For prototypes
 
void led_setup(void) {
#ifdef LED_STATE_SUPPORT

  // Led pins as output
  LED_DDR_A |= (1 << LED_RUN_A) | (1 << LED_STOP_A);
  LED_DDR_B |= (1 << LED_CPU_B) | (1 << LED_RAM_B) | (1 << LED_ROM_B);
  
  // Activity pins to LOW
  LED_PORT_B &= ~((1 << LED_CPU_B) | (1 << LED_RAM_B) | (1 << LED_ROM_B));
  
  // Run pin to LOW, Stop pin to HIGH
  LED_PORT_A |= (1 << LED_STOP_A);
  LED_PORT_A &= ~(1 << LED_RUN_A);
  
#endif
}

void led_stop_write(uint8_t state) {
#ifdef LED_STATE_SUPPORT

  if(state)
    LED_PORT_A |= (1 << LED_STOP_A);
  else
    LED_PORT_A &= ~(1 << LED_STOP_A);
	
#endif
}

void led_run_write(uint8_t state) {
#ifdef LED_STATE_SUPPORT

  if(state)
    LED_PORT_A |= (1 << LED_RUN_A);
  else
    LED_PORT_A &= ~(1 << LED_RUN_A);
	
#endif
}

void led_rom_write(uint8_t state) {
#ifdef LED_STATE_SUPPORT

  if(state)
    LED_PORT_B |= (1 << LED_ROM_B);
  else
    LED_PORT_B &= ~(1 << LED_ROM_B);
	
#endif
}

void led_ram_write(uint8_t state) {
#ifdef LED_STATE_SUPPORT

  if(state)
    LED_PORT_B |= (1 << LED_RAM_B);
  else
    LED_PORT_B &= ~(1 << LED_RAM_B);
	
#endif
}

void led_cpu_write(uint8_t state) {
#ifdef LED_STATE_SUPPORT

  if(state)
    LED_PORT_B |= (1 << LED_CPU_B);
  else
    LED_PORT_B &= ~(1 << LED_CPU_B);
	
#endif
}