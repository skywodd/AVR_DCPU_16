/**
 * Button controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _BUTTON_H_
#define _BUTTON_H_

/**
 * Setup button pins
 */
void button_setup(void);

/**
 * Get button state
 *
 * @return Button state
 */
uint8_t button_get_state(void);

/**
 * Set button state
 *
 * @param state Button state
 */
void button_set_state(uint8_t state);

#endif