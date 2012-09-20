/**
 * 23K256 RAM chip controller for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _RAM_H_
#define _RAM_H_

/**
 * Setup RAM pins
 */
void ram_setup(void);

/**
 * Read word from RAM
 *
 * @param address Address of word to read
 * @return Word read from RAM
 */
uint16_t ram_read(uint16_t address);

/**
 * Write word to RAM
 *
 * @param address Address of word to write
 * @param data Word to write
 */
void ram_write(uint16_t address, uint16_t data);

#endif