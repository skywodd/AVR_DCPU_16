/**
 * @file pinmap.h
 * @brief Pin mapping for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This configuration file define all pins used by the DCPU emulator.\n
 * @warning Do not modify this file if you don't known what you make !\n
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
 
#ifndef _PINMAP_H_
#define _PINMAP_H_

/* Pin map for ATmega328p only */
#ifdef __AVR_ATmega328P__ 

/* SPI */
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_SS   PIN2
#define SPI_SCK  PIN5
#define SPI_MISO PIN4
#define SPI_MOSI PIN3

/* UART */
#define UART_DDR DDRD
#define UART_RX  PIN0
#define UART_TX  PIN1

/* RAM (4 x 32Ko banks) (SPI) */
#define RAM_PORT      PORTC
#define RAM_DDR       DDRC
#define RAM_BANK_A_SS PIN0
#define RAM_BANK_B_SS PIN1
#define RAM_BANK_C_SS PIN2
#define RAM_BANK_D_SS PIN3

/* ROM (1x 128Ko bank) (SPI) */
#define ROM_PORT PORTC
#define ROM_DDR  DDRC
#define ROM_SS   PIN4

/* MicroVGA (Screen & Keyboard) (UART) */
#define MICROVGA_PORT      PORTD
#define MICROVGA_PIN       PIND
#define MICROVGA_DDR       DDRD
#define MICROVGA_CS_PORT   PORTC
#define MICROVGA_CS_DDR    DDRC
#define MICROVGA_CS        PIN5
#define MICROVGA_RTS       PIN3
#define MICROVGA_CTS       PIN4

/* LED (RAM, ROM, CPU, STATE status) (GPIO) */
#define LED_PORT_A PORTB
#define LED_DDR_A  DDRB
#define LED_PORT_B PORTD
#define LED_DDR_B  DDRD
#define LED_RUN_A  PIN1
#define LED_STOP_A PIN2
#define LED_CPU_B  PIN5
#define LED_ROM_B  PIN6
#define LED_RAM_B  PIN7

/* Button run/pause (GPIO) */
#define BUTTON_PORT      PORTD
#define BUTTON_DDR       DDRD
#define BUTTON_PIN       PIN2 // interrupt
#define BUTTON_INT       INT0
#define BUTTON_INT_vect  INT0_vect
#define BUTTON_INT_MASK  ((1 << ISC01) | (1 << ISC00))
#define BUTTON_INT_SENSE ((1 << ISC01) | (0 << ISC00))

/* Buzzer (GPIO) */
#define BUZZER_PORT PORTB
#define BUZZER_DDR  DDRB
#define BUZZER_PIN  PIN0

#else
#error This pin mapping is coded for ATmega328p only !
#endif

#endif