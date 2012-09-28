/**
 * @file buzzer.h
 * @brief Buzzer controller for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This controller is designed to handle DCPU-16 emulator buzzer.\n
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
 
#ifndef _BUZZER_H_
#define _BUZZER_H_

/**
 * Setup Buzzer pin and timers
 *
 * @param frequency Frequency of beep (in Hz)
 * @param duration Duration of beep (in ms)
 * @warning Maximum duration of beep is 1 second !
 */
void buzzer_setup(uint32_t frequency, uint16_t duration);

/**
 * Start a beep
 */
void buzzer_beep(void);

#endif