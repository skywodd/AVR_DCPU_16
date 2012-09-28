/**
 * @file spi.h
 * @brief SPI port controller for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This controller is designed to handle all communications made with the SPI bus.\n
 * @remarks This controller has not been tested yet, but it should work anyway
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

#ifndef _SPI_H_
#define _SPI_H_

/**
 * Supported SPI clock divider
 *
 * @var spi_clock
 */
typedef enum {
  SPI_CLOCK_DIV_BY_4,
  SPI_CLOCK_DIV_BY_16,
  SPI_CLOCK_DIV_BY_64,
  SPI_CLOCK_DIV_BY_128,
  SPI_CLOCK_DIV_BY_2,
  SPI_CLOCK_DIV_BY_8,
  SPI_CLOCK_DIV_BY_32
} spi_clock;

/**
 * Supported SPI mode
 *
 * @var spi_mode
 */
typedef enum {
  SPI_MODE_0,
  SPI_MODE_1 = 0x04,
  SPI_MODE_2 = 0x08,
  SPI_MODE_3 = 0x0C
} spi_mode;

/**
 * Supported SPI bits order mode
 * 
 * @var spi_bits_order
 */
typedef enum {
  LSBFIRST,
  MSBFIRST
} spi_bits_order;

/**
 * Setup SPI port and pins
 *
 * @param prescaler Clock divider of SPI port
 * @param mode Operation mode of SPI port
 * @param order Bits order of SPI port
 */
void spi_setup(spi_clock prescaler, spi_mode mode, spi_bits_order order);

/**
 * Transfer a byte to SPI slave
 *
 * @param data Byte to send to SPI slave
 * @return Byte read from SPI slave
 */
uint8_t spi_transfer(uint8_t data);

#endif