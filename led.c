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