/**
 * Status led controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _LED_H_
#define _LED_H_

/**
 * Setup led pins
 */
void led_setup(void);

/**
 * Set stop led status
 *
 * @param state led status
 */
void led_stop_write(uint8_t state);

/**
 * Set run led status
 *
 * @param state led status
 */
void led_run_write(uint8_t state);

/**
 * Set rom activity led status
 *
 * @param state led status
 */
void led_rom_write(uint8_t state);

/**
 * Set ram activity led status
 *
 * @param state led status
 */
void led_ram_write(uint8_t state);

/**
 * Set cpu activity led status
 *
 * @param state led status
 */
void led_cpu_write(uint8_t state);

#endif