/**
 * Button controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */

/* Includes */ 
#include <avr/io.h>        // For gpio purpose
#include <util/delay.h>    // For cycles delay purpose
#include <avr/interrupt.h> // For interruptions
#include <stdint.h>        // For hardcoded int type
#include "led.h"           // For led controller
#include "pinmap.h"        // For pin mapping
#include "debug.h"         // For debug
#include "button.h"        // For prototypes

/* Global variables */
#ifdef BUTTON_STATE_SUPPORT
static volatile uint8_t bp_state = 0;
#endif

/* Interrupts */
// Play / Pause button
#ifdef BUTTON_STATE_SUPPORT
ISR(BUTTON_INT_vect) { 

  bp_state = !bp_state;      // Toggle button state
  led_run_write(bp_state);   // Ping back user with status led
  led_stop_write(!bp_state); // Ping back user with status led
  DEBUG_STR("Button", "ISR trigged");
  
}
#endif

void button_setup(void) {
#ifdef BUTTON_STATE_SUPPORT

  // Button pin as input
  BUTTON_DDR &= ~(1 << BUTTON_PIN);
  
  // Enable pull-up on button pin
  BUTTON_PORT |= (1 << BUTTON_PIN);
  
  // Setup interrupt sense
  EICRA = (EICRA & ~BUTTON_INT_MASK) | BUTTON_INT_SENSE;
  
  // Enable interrupt
  EIMSK |= (1 << BUTTON_INT);
  
#endif
}

uint8_t button_get_state(void) {
#ifdef BUTTON_STATE_SUPPORT

  return bp_state; // Return button state
  
#else

  return 1;        // Return true
  
#endif
}

void button_set_state(uint8_t state) {
#ifdef BUTTON_STATE_SUPPORT

  bp_state = state;          // Set button state
  led_run_write(bp_state);   // Ping back user with status led
  led_stop_write(!bp_state); // Ping back user with status led
  
#endif
}