/**
 * VM core for DCPU project
 *
 * @author Skywodd
 * @link http://skyduino.wordpress.com
 */
 
#ifndef _DCPU_H_
#define _DCPU_H_

/**
 * DCPU registers structure 
 *
 * @var DCPU_registers
 */
typedef struct {
  uint16_t rA, rB, rC, rX, rY, rZ, rI, rJ, rPC, rSP, rO;
} DCPU_registers;

/**
 * Initialised DCPU registers structure with default values
 *
 * @param init_reg Registers to init
 */
void dcpu_register_init(DCPU_registers *init_reg);

#ifdef ONBOARD_MEMORY
#define DCPU_REGISTER_INIT {0, 0, 0, 0, 0, 0, 0, 0, ONBOARD_RAM_SIZE - 1, 0}
#else
#define DCPU_REGISTER_INIT {0, 0, 0, 0, 0, 0, 0, 0, 0xFFFF, 0}
#endif

/**
 * Setup the DCPU-16 core
 *
 * @param init_reg Registers values to use at start time
 */
void dcpu_setup(DCPU_registers init_reg);

/**
 * Dump DCPU-16 registers states
 *
 * @return Registers states (at function call)
 */
DCPU_registers dcpu_registers_dump(void);

/**
 * Step DCPU-16 core (fetch one opcode and execute)
 */
void dcpu_step(void);

#endif