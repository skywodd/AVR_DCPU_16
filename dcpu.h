/**
 * @file dcpu.h
 * @brief Virtual CPU core for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This functions bundle implement everything related to DCPU-16 emulation stuff.\n
 * This bundle is generic and can be ported to any platform that support C language.\n
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

#ifndef _DCPU_H_
#define _DCPU_H_

/**
 * DCPU registers structure 
 *
 * @struct DCPU_registers
 */
typedef struct {
  uint16_t rA, rB, rC, rX, rY, rZ, rI, rJ, rPC, rSP, rO;
} DCPU_registers;

/**
 * Initialise DCPU registers structure with default values
 *
 * @param init_reg Registers to init
 */
void dcpu_register_init(DCPU_registers *init_reg);

#ifdef ONBOARD_MEMORY
#define DCPU_REGISTER_INIT {0, 0, 0, 0, 0, 0, 0, 0, ONBOARD_RAM_SIZE - 1, 0}
#else
#define DCPU_REGISTER_INIT {0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF, 0}
#endif

/**
 * Setup the DCPU-16 core
 *
 * @param init_reg Registers values to use at start time
 */
void dcpu_setup(DCPU_registers init_reg);

/**
 * Dump DCPU-16 registers states
 *
 * @return Registers states (at function call)
 */
DCPU_registers dcpu_registers_dump(void);

/**
 * Step DCPU-16 core (fetch one opcode from memory and execute them)
 */
void dcpu_step(void);

#endif