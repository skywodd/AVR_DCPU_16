/**
 * Buzzer controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
/* Includes */
#include <avr/io.h>        // For gpio purpose
#include <avr/interrupt.h> // For interruptions
#include <stdint.h>        // For hardcoded int type
#include "pinmap.h"        // For pin mapping
#include "debug.h"         // For debug
#include "buzzer.h"        // For prototypes

/* Global variables */
static volatile uint8_t timer0_prescaler = 0;
static volatile uint8_t timer1_prescaler = 0;

/* Interrupts */
#ifdef BUZZER_BEEP_SUPPORT
// Beep square wave generation
ISR(TIMER0_COMPA_vect) {

  BUZZER_PORT ^= (1 << BUZZER_PIN); // Toggle buzzer pin
  
}

// Beep duration handler
ISR(TIMER1_COMPA_vect) { 
  
  // Timers clock disable
  TCCR0B = (0 << WGM02);                // Timer0 off
  TCCR1B = (0 << WGM13) | (1 << WGM12); // Timer1 off
  
  // Timers interrupts disable
  TIMSK0 = (0 << OCIE0A);
  TIMSK1 = (0 << OCIE1A);
  
  // Buzzer pin disable (avoid glitchs)
  BUZZER_PORT &= ~(1 << BUZZER_PIN);    // Buzzer pin to LOW
  DEBUG_STR("Buzzer", "beep stop");
  
}
#endif

void buzzer_setup(uint32_t frequency, uint16_t duration) {
#ifdef BUZZER_BEEP_SUPPORT

  DEBUG_STR("Buzzer", "setup");
  // Buzzer pin as output
  BUZZER_DDR |= (1 << BUZZER_PIN);
  BUZZER_PORT &= ~(1 << BUZZER_PIN);

  // Compute raw frequency to interrupts frequency
  frequency *= 2;

  // CTC mode
  TCCR0A = (1 << WGM01) | (0 << WGM00);
  TCCR0B = (0 << WGM02); // + Timer0 off
  
  // CTC mode
  TCCR1A = (0 << WGM11) | (0 << WGM10);
  TCCR1B = (0 << WGM13) | (1 << WGM12); // + Timer1 off
  TCCR1C = 0;
  
  // Compute timer0 prescaler
  frequency = F_CPU / frequency; // Pre-compute frequency 
  timer0_prescaler = 0b001;
  uint16_t ocr0 = frequency / 1 - 1; // Prescaler /1
  if(ocr0 > 255) {
    timer0_prescaler = 0b010;
	ocr0 = frequency / 8 - 1; // Prescaler /8
	
    if(ocr0 > 255) {
      timer0_prescaler = 0b011;
	  ocr0 = frequency / 64 - 1; // Prescaler /64
	  
      if(ocr0 > 255) {
        timer0_prescaler = 0b100;
	    ocr0 = frequency / 256 - 1; // Prescaler /256
		
        if(ocr0 > 255) {
          timer0_prescaler = 0b101;
	      ocr0 = frequency / 1024 - 1; // Prescaler /1024
		  
          if(ocr0 > 255) {
            ocr0 = 255; 
          }
        }
      }
    }
  }
  OCR0A = ocr0;
  DEBUG_BIN("Buzzer freq prescaler", timer0_prescaler);
  DEBUG_DEC("Buzzer freq ocrx", ocr0);
  
  uint32_t cycles_duration = F_CPU / (1000 / duration);
  timer1_prescaler = 0b001;
  uint32_t ocr1 = cycles_duration / 1 - 1; // Prescaler /1
  if(ocr1 > 65535) {
    timer1_prescaler = 0b010;
	ocr1 = cycles_duration / 8 - 1; // Prescaler /8
	
    if(ocr1 > 65535) {
      timer1_prescaler = 0b011;
	  ocr1 = cycles_duration / 64 - 1; // Prescaler /64
	  
      if(ocr1 > 65535) {
        timer1_prescaler = 0b100;
	    ocr1 = cycles_duration / 256 - 1; // Prescaler /256
		
        if(ocr1 > 65535) {
          timer1_prescaler = 0b101;
	      ocr1 = cycles_duration / 1024 - 1; // Prescaler /1024
		  
          if(ocr1 > 65535) {
            ocr1 = 65535; 
          }
        }
      }
    }
  }
  OCR1AH = (uint8_t)(ocr1 >> 8);
  OCR1AL = (uint8_t)ocr1;
  DEBUG_BIN("Buzzer duration prescaler", timer1_prescaler);
  DEBUG_DEC("Buzzer duration ocrx", ocr1);
  DEBUG_STR("Buzzer", "setup done");
  
#endif
}

void buzzer_beep(void) {
#ifdef BUZZER_BEEP_SUPPORT

  TCNT0 = 0;  // Timer0 counter reset
  TCNT1H = 0; // Timer1 counter reset
  TCNT1L = 0; // 
  
  // Enable output compare match A on Timer0 & Timer1
  TIMSK0 = (1 << OCIE0A);
  TIMSK1 = (1 << OCIE1A);
  
  TCCR0B = (0 << WGM02) | timer0_prescaler; // Turn on timer0
  TCCR1B = (0 << WGM13) | (1 << WGM12) | timer1_prescaler; // Turn on timer1
  DEBUG_STR("Buzzer", "beep start");
  
#endif
}