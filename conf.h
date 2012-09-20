/**
 * Configuration file for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _CONF_H_
#define _CONF_H_

// Makefile integrity check
#ifndef __EXTRA_DEFINE__
#error You re doing it wrong ! Check your makefile !
#endif

// On board memory mode configuration
#define ONBOARD_RAM_SIZE 448
#define ONBOARD_STACK_SIZE 64
#define ONBOARD_ROM_SIZE 43//45

// SPI configuration
#define SPI_PRESCALER  SPI_CLOCK_DIV_BY_8
#define SPI_MODE       SPI_MODE_0
#define SPI_BITS_ORDER MSBFIRST

// Buzzer configuration
#define BUZZER_FREQUENCY 500UL
#define BUZZER_DURATION  500UL

// UART configuration
#define UART_BAUDRATE 9600UL

#endif