/**
 * Pin mapping for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _PINMAP_H_
#define _PINMAP_H_

#ifdef __AVR_ATmega328P__ // Pin map for ATmega328p only

// SPI
#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_SS   PIN2
#define SPI_SCK  PIN5
#define SPI_MISO PIN4
#define SPI_MOSI PIN3

// UART
#define UART_DDR DDRD
#define UART_RX  PIN0
#define UART_TX  PIN1

// RAM (4 x 32Ko banks) (SPI)
#define RAM_PORT      PORTC
#define RAM_DDR       DDRC
#define RAM_BANK_A_SS PIN0
#define RAM_BANK_B_SS PIN1
#define RAM_BANK_C_SS PIN2
#define RAM_BANK_D_SS PIN3

// ROM (1x 128Ko bank) (SPI)
#define ROM_PORT PORTC
#define ROM_DDR  DDRC
#define ROM_SS   PIN4

// MicroVGA (Screen & Keyboard) (UART)
#define MICROVGA_PORT      PORTD
#define MICROVGA_PIN       PIND
#define MICROVGA_DDR       DDRD
#define MICROVGA_CS_PORT   PORTC
#define MICROVGA_CS_DDR    DDRC
#define MICROVGA_CS        PIN5
#define MICROVGA_RTS       PIN3
#define MICROVGA_CTS       PIN4

// LED (RAM, ROM, CPU, STATE status) (GPIO)
#define LED_PORT_A PORTB
#define LED_DDR_A  DDRB
#define LED_PORT_B PORTD
#define LED_DDR_B  DDRD
#define LED_RUN_A  PIN1
#define LED_STOP_A PIN2
#define LED_CPU_B  PIN5
#define LED_ROM_B  PIN6
#define LED_RAM_B  PIN7

// Button run/pause (GPIO)
#define BUTTON_PORT      PORTD
#define BUTTON_DDR       DDRD
#define BUTTON_PIN       PIN2 // interrupt
#define BUTTON_INT       INT0
#define BUTTON_INT_vect  INT0_vect
#define BUTTON_INT_MASK  ((1 << ISC01) | (1 << ISC00))
#define BUTTON_INT_SENSE ((1 << ISC01) | (0 << ISC00))

// Buzzer (GPIO)
#define BUZZER_PORT PORTB
#define BUZZER_DDR  DDRB
#define BUZZER_PIN  PIN0

#else
#error This pin mapping is coded for ATmega328p only !
#endif

#endif