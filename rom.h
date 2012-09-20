/**
 * A25L010 Flash chip controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _ROM_H_
#define _ROM_H_

/**
 * Setup ROM pins 
 */
void rom_setup(void);

/**
 * Read word from ROM
 *
 * @param address Address of word to read
 * @return Word read from ROM
 */
uint16_t rom_read(uint16_t address);

#endif