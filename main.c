/**
 * @file main.c
 * @brief Hardware implementation of notch DCPU-16 on AVR mcu
 * @mainpage 
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This program is a hardware implementation of the DCPU-16 from Notch (creator of minecraft game) on AVR mcu.\n
 * This program is designed to run on an ATmega328p with a MicroVGA board for VGA display.\n
 * \n
 * Please report bug to <skywodd at gmail.com>
 *
 * @note More details of the MicroVGA board are available in the link below :
 * @see http://microvga.com/
 *
 * @note DCPU-16 specifications are available in the link below :
 * @see http://0x10c.com/doc/dcpu-16.txt
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
 *
 * @section changelog_sec Changelog history
 * - 21/07/2012 : First version
 *
 * @section other_sec Schematic
 * @image html schematic.png
 *
 * @remarks This program does not attempt to follow the lastest DCPU-16 specifications !
 * @warning This program implement DCPU-16 specifications v1.1
 */
 
/* Includes */
#include <stdint.h>        // For hardcoded int type
#include <avr/interrupt.h> // For interrupts
#include <util/delay.h>    // For delay
#include "button.h"        // For button controller
#include "buzzer.h"        // For buzzer controller
#include "dcpu.h"          // For DCPU routines
#include "led.h"           // For led controller
#include "microvga.h"      // For MicroVGA controller
#include "ram.h"           // For RAM controller
#include "rom.h"           // For ROM controller
#include "spi.h"           // For SPI controller
#include "uart.h"          // For UART controller
#include "debug.h"         // For debug
#include "conf.h"          // For hardware configuration

/**
 * Entry point 
 */
int main(void) {

  /* Misc variables */
  DCPU_registers reg; // CPU registers states (at boot)

  /* Hardware initialisation */
  cli();
  led_setup();
  spi_setup(SPI_PRESCALER, SPI_MODE, SPI_BITS_ORDER);
  uart_setup(UART_BAUDRATE);
  DEBUG_STR("Main init", "UART ready");
  button_setup();
  buzzer_setup(BUZZER_FREQUENCY, BUZZER_DURATION);
  ram_setup();
  rom_setup();
  microvga_setup();
  microvga_enable();
  dcpu_register_init(&reg);
  DEBUG_STR("Main init", "done");
  sei();
  
  /* MicroVGA initialisation */
  _delay_ms(1000);         // MicroVGA boot time
  microvga_clear_screen(); // Clear screen and goto (0, 0)
  microvga_goto_cursor(0, 0);
  uart_puts_PSTR(PSTR("SkyWodd DCPU-16 hardware emulator")); // Screen test
  buzzer_beep();
  DEBUG_STR("Main init", "MicroVGA ready");
  
  /* Hardware self-test */
  DEBUG_STR("Main init", "self-test run");
  led_run_write(1); // Led test
  _delay_ms(250);
  led_run_write(0);
  led_cpu_write(1);
  _delay_ms(250);
  led_cpu_write(0);
  led_rom_write(1);
  _delay_ms(250);
  led_rom_write(0);
  led_ram_write(1);
  _delay_ms(250);
  led_ram_write(0);
  DEBUG_STR("Main init", "self-test done");
  
  /* Keyboard & MicroVGA api test */
  DEBUG_STR("Main init", "waiting for keypress");
  microvga_goto_cursor(0, 1);
  uart_puts_PSTR(PSTR("Press any key to boot ..."));
  keyboard_wait();
  uart_puts_PSTR(PSTR("Loading please wait ..."));
  dcpu_setup(reg);
  buzzer_beep();
  microvga_clear_screen();
  microvga_goto_cursor(0, 0);
  DEBUG_STR("Main init", "ready to run");
  
  /* Infinite loop */
  for(;;) {
  
    /* Handle pause */
    while(!button_get_state()); 
	
#ifdef SERIAL_DEBUG_SUPPORT
	/* Debug */
	dcpu_registers_dump();
#endif
	
	/* Fetch opcode from ROM */
    dcpu_step(); 
  }
  
}