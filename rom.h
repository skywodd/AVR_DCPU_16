/**
 * @file rom.h
 * @brief ROM controller for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This controller is designed to handle ROM memory operations.\n
 * This controller can drive A25L010 flash chip or use internal ROM in ONBOARD mode.\n
 * @warning NOTE TESTED YET !
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
 
#ifndef _ROM_H_
#define _ROM_H_

/**
 * Setup ROM pins 
 */
void rom_setup(void);

/**
 * Read word from ROM
 *
 * @param address Address of word to read
 * @return Word read from ROM
 */
uint16_t rom_read(uint16_t address);

#endif