/**
 * @file conf.h
 * @brief Configuration file for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This configuration file is designed to let user choose his hardware configuration as they want.\n
 * Default values are well tested, change them only if you known what you make.\n
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
 
#ifndef _CONF_H_
#define _CONF_H_

/* Makefile integrity check */
#ifndef __EXTRA_DEFINE__
#error You re doing it wrong ! Check your makefile !
#endif

/* On board memory mode configuration */
/** On board ram size in internal-memory mode */
#define ONBOARD_RAM_SIZE 448
/** On board stack size in internal-memory mode */
#define ONBOARD_STACK_SIZE 64
/** On board rom size in internal-memory mode */
#define ONBOARD_ROM_SIZE 43//45

/* SPI configuration */
/** SPI clock prescaler */
#define SPI_PRESCALER  SPI_CLOCK_DIV_BY_8
/** SPI mode */
#define SPI_MODE       SPI_MODE_0
/** SPI bits order mode */
#define SPI_BITS_ORDER MSBFIRST

/* Buzzer configuration */
/** Beep frequency */
#define BUZZER_FREQUENCY 500UL
/** Beep duration */
#define BUZZER_DURATION  500UL

/* UART configuration */
/** UART baudrate */
#define UART_BAUDRATE 9600UL

#endif