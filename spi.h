/**
 * SPI port controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
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