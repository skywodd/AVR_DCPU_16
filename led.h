/**
 * @file led.h
 * @brief Status led controller for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This controller is designed to handle status led of DCPU emulator.\n
 * \n
 * Please report bug to <skywodd at gmail.com>
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
 */
 
#ifndef _LED_H_
#define _LED_H_

/**
 * Setup led pins
 */
void led_setup(void);

/**
 * Set stop led state
 *
 * @param state Led state
 */
void led_stop_write(uint8_t state);

/**
 * Set run led state
 *
 * @param state Led state
 */
void led_run_write(uint8_t state);

/**
 * Set rom activity led state
 *
 * @param state Led state
 */
void led_rom_write(uint8_t state);

/**
 * Set ram activity led state
 *
 * @param state Led state
 */
void led_ram_write(uint8_t state);

/**
 * Set cpu activity led state
 *
 * @param state Led state
 */
void led_cpu_write(uint8_t state);

#endif