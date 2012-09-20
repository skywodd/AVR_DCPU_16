/**
 * Buzzer controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _BUZZER_H_
#define _BUZZER_H_

/**
 * Setup Buzzer pin and timers
 *
 * @param frequency Frequency of beep (in Hz)
 * @param duration Duration of beep (in ms, /!\ maximum 1 second)
 */
void buzzer_setup(uint32_t frequency, uint16_t duration);

/**
 * Start a beep
 */
void buzzer_beep(void);

#endif