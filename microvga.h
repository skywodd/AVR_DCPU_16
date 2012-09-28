/**
 * @file microvga.h
 * @brief MicroVGA controller for DCPU project
 * @author SkyWodd
 * @version 1.0
 * @see http://skyduino.wordpress.com/
 *
 * @section intro_sec Introduction
 * This controller is designed to handle communiactions with MicroVGA display board.\n
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

#ifndef _MICROVGA_H_
#define _MICROVGA_H_

/**
 * MicroVGA supported color mode
 *
 * @var microvga_color
 */
typedef enum { 
  COLOR_BLACK,
  COLOR_RED,
  COLOR_GREEN,
  COLOR_YELLOW,
  COLOR_BLUE,
  COLOR_MAGENTA,
  COLOR_CYAN,
  COLOR_WHITE
} microvga_color;

/**
 * MicroVGA supported attribute setting
 *
 * @var microvga_attribute
 */
typedef enum {
  ATTRIBUTE_RESET_ALL,
  ATTRIBUTE_BRIGHT_FOREGROUND,
  ATTRIBUTE_DIM_FOREGROUND,
  ATTRIBUTE_BRIGHT_BACKGROUND,
  ATTRIBUTE_BLINK_MODE,
  ATTRIBUTE_NOBLINK_MODE,
  ATTRIBUTE_DIM_BACKGROUND = 6,
  ATTRIBUTE_REVERSE_MODE,
  ATTRIBUTE_HIDDEN_MODE
} microvga_attribute;

/**
 * MicroVGA supported cursor move operation
 *
 * @var microvga_move
 */
typedef enum {
  MOVE_UP,
  MOVE_DOWN,
  MOVE_FORWARD,
  MOVE_BACKWARD
} microvga_move;

/**
 * MicroVGA supported font page
 *
 * @var microvga_font
 */
typedef enum {
  US_ASCII, 
  CP_437 = 0,
  ISO_8859_1,
  ISO_8859_2,
  ISO_8859_3,
  ISO_8859_4,
  ISO_8859_5
} microvga_font;

/**
 * Setup MicroVGA pins
 */
void microvga_setup(void);

/**
 * Enable MicroVGA communications
 */
void microvga_enable(void);

/**
 * Disable MicroVGA communications
 */
void microvga_disable(void);

/**
 * Hide blinking cursor
 */
void microvga_hide_cursor(void);

/**
 * Show blinking cursor
 */
void microvga_show_cursor(void);

/**
 * Set textmode attribute
 *
 * @param attribute Attribute to set
 */
void microvga_set_attribute(microvga_attribute attribute);

/**
 * Set foreground text color
 *
 * @param color Color of foreground text
 */
void microvga_set_foreground_color(microvga_color color);

/**
 * Set background text color
 *
 * @param color Color of background text
 */
void microvga_set_background_color(microvga_color color);

/**
 * Clear End Of Line
 */
void microvga_clear_eol(void);

/**
 * Clear screen
 */
void microvga_clear_screen(void);

/**
 * Move cursor to (x, y) position
 *
 * @note XY positions start at index 0
 * @param x X position (collumn)
 * @param y Y position (row)
 */
void microvga_goto_cursor(uint8_t x, uint8_t y);

/**
 * Move cursor in a specified direction
 *
 * @param move Direction of move
 * @param count Numbers of move (in row or collumn)
 */
void microvga_move_cursor(microvga_move move, uint8_t count);

/**
 * Set font page of display
 *
 * @param font Font page to use
 */
void microvga_set_font(microvga_font font);

/**
 * Interpret DCPU-16 screen word and apply required screen operations
 *
 * @param offset Memory offset of data-word (start at h000, finish at h200)
 * @param data Data-word to interpret
 */
void screen_word_interpret(uint16_t offset, uint16_t data);

/**
 * Wait for a key to be pressed
 */
void keyboard_wait(void);

/**
 * Set the keyboard key pressed flag
 */
void keyboard_flag_set(void);

/**
 * Get the state of RTS flow control
 *
 * @return The state or RTS line
 */
uint8_t microvga_ready(void);

#endif