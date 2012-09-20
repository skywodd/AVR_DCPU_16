/**
 * SPI port controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
/* Includes */
#include <avr/io.h> // For GPIO
#include <stdint.h> // For hardcoded int type
#include "pinmap.h" // For pin mapping
#include "spi.h"    // For prototypes

void spi_setup(spi_clock prescaler, spi_mode mode, spi_bits_order order) {

  // SS, SCK, MOSI pins as output
  SPI_DDR |= (1 << SPI_SS) | (1 << SPI_SCK) | (1 << SPI_MOSI);
  
  // SCK and MOSI to LOW (SPI in idle state)
  SPI_PORT &= ~((1 << SPI_SCK) | (1 << SPI_MOSI));
  
#ifdef HARDWARE_SPI_SS_SUPPORT
  // SS pin to HIGH (unselect SPI device)
  SPI_PORT |= (1 << SPI_SS);
#endif

  // MISO pin as input
  SPI_DDR &= ~(1 << SPI_MISO);
  
  // Set SPI clock divider
  SPCR = (SPCR & ~0x03) | (prescaler & 0x03);
  SPSR = (SPSR & ~0x01) | ((prescaler >> 2) & 0x01);

  // Set SPI mode
  SPCR = (SPCR & ~0x0C) | mode;
  
  // Set SPI bits order
  if(order == LSBFIRST)
    SPCR |= (1 << DORD);
  else
    SPCR &= ~(1 << DORD);
	
  // Enable SPI
  SPCR |= (1 << SPE) | (1 << MSTR);
  
}

uint8_t spi_transfer(uint8_t data) {

  SPDR = data;                   // Send byte
  while (!(SPSR & (1 << SPIF))); // Wait for transfer to finish
  return SPDR;                   // Return SPI slave answer
  
}