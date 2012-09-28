/*
 * See header file for details
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
 */
 
/* Includes */
#include <stdint.h> // For hardcoded int type
#include "led.h"    // For led controller
#include "button.h" // For button controller
#include "rom.h"    // For RAM
#include "ram.h"    // For ROM
#include "conf.h"   // For Configuration
#include "debug.h"  // For debug
#include "dcpu.h"   // For prototypes

/* Prototypes */
uint16_t dcpu_register_code_read(uint8_t reg_code);             // Return register value pointed by reg_code
void dcpu_register_code_write(uint8_t reg_code, uint16_t data); // Set register value pointed by reg_code

/* Global */
static volatile DCPU_registers internal_dcpu_reg;
static uint8_t DCPU_next_instruction_skip = 0;
static uint8_t DCPU_PC_offset = 0;

/* Disassembly macro */
#define GET_OPCODE(x) (x & 0x000F)        // Get Op-code from data word
#define GET_ACODE(x) ((x & 0x03F0) >> 4)  // Get A-code from data word
#define GET_BCODE(x) ((x & 0xFC00) >> 10) // Get B-code from data word

void dcpu_register_init(DCPU_registers *init_reg) {

  init_reg->rA = 0;
  init_reg->rB = 0;
  init_reg->rC = 0;
  init_reg->rX = 0;
  init_reg->rY = 0;
  init_reg->rZ = 0;
  init_reg->rI = 0;
  init_reg->rJ = 0;
  init_reg->rPC = 0;
  init_reg->rSP = 0;
  init_reg->rO = 0;
  DEBUG_HEX("DCPU register init address", (uint16_t)init_reg);
  
}

void dcpu_setup(DCPU_registers init_reg) {

  DEBUG_STR("DCPU memory", "copy rom to ram");
  uint16_t data, i = 0;
  do {                  // Copy ROM to RAM (instanciate programm)
#ifdef ONBOARD_MEMORY
  if(i == ONBOARD_ROM_SIZE) break;
#endif
    data = rom_read(i); //
    ram_write(i, data); //
  } while(++i > 0);     // Use uint16_t overflow as stop condition
  DEBUG_STR("DCPU memory", "done");

  DEBUG_STR("DCPU registers", "setup");
  internal_dcpu_reg = init_reg; // Set registers values
  
}

DCPU_registers dcpu_registers_dump(void) {

  DEBUG_STR("DCPU registers", "dump");
  DEBUG_DEC("DCPU register A", internal_dcpu_reg.rA);
  DEBUG_DEC("DCPU register B", internal_dcpu_reg.rB);
  DEBUG_DEC("DCPU register C", internal_dcpu_reg.rC);
  DEBUG_DEC("DCPU register X", internal_dcpu_reg.rX);
  DEBUG_DEC("DCPU register Y", internal_dcpu_reg.rY);
  DEBUG_DEC("DCPU register Z", internal_dcpu_reg.rZ);
  DEBUG_DEC("DCPU register I", internal_dcpu_reg.rI);
  DEBUG_DEC("DCPU register J", internal_dcpu_reg.rJ);
  DEBUG_HEX("DCPU register SP", internal_dcpu_reg.rSP);
  DEBUG_HEX("DCPU register PC", internal_dcpu_reg.rPC);
  DEBUG_DEC("DCPU register O", internal_dcpu_reg.rO);
  
  return internal_dcpu_reg;     // Return current registers values
  
}

void dcpu_step(void) {

  // Temp variable for calculation
  uint16_t temp = 0;

  led_cpu_write(1); // Turn on cpu activity led
  
  // Read opcode from rom 
  uint16_t instruction = ram_read(internal_dcpu_reg.rPC++); // rom?
  DCPU_PC_offset = 0; // Reset PC offset before handling _VA & _VB
  DEBUG_HEX("DCPU ROM instruction", instruction); 
	
  // Compute register code
  uint8_t _A = GET_ACODE(instruction);
  uint8_t opcode = GET_OPCODE(instruction);
  uint16_t _VA = dcpu_register_code_read(_A);
  uint16_t _VB = dcpu_register_code_read(GET_BCODE(instruction));
  
  DEBUG_HEX("DCPU opcode", opcode);
  DEBUG_DEC("DCPU acode", _A);
  DEBUG_DEC("DCPU bcode", GET_BCODE(instruction));
  DEBUG_HEX("DCPU acode pointed register value", _VA);
  DEBUG_HEX("DCPU bcode pointed register value", _VB);
    
  if(DCPU_next_instruction_skip && opcode) {
    DEBUG_STR("DCPU interpret", "will be ignored (next_instruction_skip flag set)");
    internal_dcpu_reg.rPC += DCPU_PC_offset; // Apply final PC offset
	DCPU_next_instruction_skip = 0;
	return;
  } 
	
  // Interpret opcode
  switch(opcode) {
    case 0x0: // Non-basic instruction
	  DEBUG_STR("DCPU interpret", "Non-basic opcode");
	  if(DCPU_next_instruction_skip && opcode) {
        DEBUG_STR("DCPU interpret", "will be ignored (next_instruction_skip flag set)");
        internal_dcpu_reg.rPC += DCPU_PC_offset;  // Apply final PC offset
	    DCPU_next_instruction_skip = 0;
		return;
      } 
      switch(_A) { // Warning : opcode expansions MUST handle DCPU_next_instruction_skip flag themself !
        case 0x00: // Reserved for future expansion
		  DEBUG_STR("DCPU interpret", "Reserved opcode");
		  button_set_state(0); // Turn on error led and stop CPU
          break;
            
        case 0x01: // JSR a - pushes the address of the next instruction to the stack, then sets PC to a
		  DEBUG_STR("DCPU interpret", "JSR");
          ram_write(--(internal_dcpu_reg.rSP), internal_dcpu_reg.rPC + DCPU_PC_offset);
          internal_dcpu_reg.rPC = _VB;
		  return; // Don't forget it, otherwise the PC offset will be setup later
          break;
		  
		default: // Unvalid opcode
		  DEBUG_STR("DCPU interpret", "ERROR");
		  button_set_state(0); // Turn on error led and stop CPU
		  break;
      }
      break;

    case 0x1: // SET a, b - sets a to b
	  DEBUG_STR("DCPU interpret", "SET");
      temp = _VB;
      break;
        
    case 0x2: // ADD a, b - sets a to a+b, sets O to 0x0001 if there's an overflow, 0x0 otherwise
	  DEBUG_STR("DCPU interpret", "ADD");
	  temp = _VA + _VB;
#ifdef O_REGISTER_SUPPORT
      internal_dcpu_reg.rO = SREG & 1; 
#endif
      break;
        
    case 0x3: // SUB a, b - sets a to a-b, sets O to 0xffff if there's an underflow, 0x0 otherwise
	  DEBUG_STR("DCPU interpret", "SUB");
      temp = _VA - _VB;
#ifdef O_REGISTER_SUPPORT
      internal_dcpu_reg.rO = (SREG & 1) ? 0xFFFF : 0; 
#endif
      break;
        
    case 0x4: // MUL a, b - sets a to a*b, sets O to ((a*b)>>16)&0xffff
	  DEBUG_STR("DCPU interpret", "MUL");
      temp = _VA * _VB;
#ifdef O_REGISTER_SUPPORT
      internal_dcpu_reg.rO = (((uint32_t)((uint32_t)_VA * (uint32_t)_VB)) >> 16) & 0xFFFF; 
#endif
      break;
        
    case 0x5: // DIV a, b - sets a to a/b, sets O to ((a<<16)/b)&0xffff. if b==0, sets a and O to 0 instead.
	  DEBUG_STR("DCPU interpret", "DIV");
      if(!_VB) {
        temp = 0;
#ifdef O_REGISTER_SUPPORT
        internal_dcpu_reg.rO = 0;
#endif
        break;
      }
      temp = _VA / _VB;
#ifdef O_REGISTER_SUPPORT
      internal_dcpu_reg.rO = ((((uint32_t)_VA) << 16) / (uint32_t)_VB) & 0xFFFF; 
#endif
      break;
        
    case 0x6: // MOD a, b - sets a to a%b. if b==0, sets a to 0 instead.
	  DEBUG_STR("DCPU interpret", "MOD");
      if(!_VB) {
        temp = 0;
        break;
      }
      temp = _VA % _VB;
      break;
        
    case 0x7: // SHL a, b - sets a to a<<b, sets O to ((a<<b)>>16)&0xffff
	  DEBUG_STR("DCPU interpret", "SHL");
      temp = _VA << _VB;
#ifdef O_REGISTER_SUPPORT
      internal_dcpu_reg.rO = (((uint32_t)((uint32_t)_VA << (uint32_t)_VB)) >> 16) & 0xFFFF; 
#endif
      break;
        
    case 0x8: // SHR a, b - sets a to a>>b, sets O to ((a<<16)>>b)&0xffff
	  DEBUG_STR("DCPU interpret", "SHR");
      temp = _VA >> _VB;
#ifdef O_REGISTER_SUPPORT
      internal_dcpu_reg.rO = (((uint32_t)_VA << 16) >> (uint32_t)_VB) & 0xFFFF; 
#endif
      break;
        
    case 0x9: // AND a, b - sets a to a&b
	  DEBUG_STR("DCPU interpret", "AND");
      temp = _VA & _VB;
      break;
        
    case 0xA: // BOR a, b - sets a to a|b
	  DEBUG_STR("DCPU interpret", "BOR");
      temp = _VA | _VB;
      break;
        
    case 0xB: // XOR a, b - sets a to a^b
	  DEBUG_STR("DCPU interpret", "XOR");
      temp = _VA ^ _VB;
      break;
        
    case 0xC: // IFE a, b - performs next instruction only if a==b
	  DEBUG_STR("DCPU interpret", "IFE");
      if(_VA != _VB) DCPU_next_instruction_skip = 1;
      break;
      
    case 0xD: // IFN a, b - performs next instruction only if a!=b
	  DEBUG_STR("DCPU interpret", "IFN");
      if(_VA == _VB) DCPU_next_instruction_skip = 1;
      break;
        
    case 0xE: // IFG a, b - performs next instruction only if a>b
	  DEBUG_STR("DCPU interpret", "IFG");
      if(_VA <= _VB) DCPU_next_instruction_skip = 1;
      break;
        
    case 0xF: // IFB a, b - performs next instruction only if (a&b)!=0
	  DEBUG_STR("DCPU interpret", "IFB");
      if(!(_VA & _VB)) DCPU_next_instruction_skip = 1;
      break;
  }
  
  // Commit write to pointed register
  if((opcode > 0x00 && opcode <= 0x0B)) {
    DEBUG_HEX("DCPU register _A commit", temp);
    dcpu_register_code_write(_A, temp);
  }
  
  // Apply final PC offset
  internal_dcpu_reg.rPC += DCPU_PC_offset;
  
  led_cpu_write(0); // Turn off cpu activity led
}

uint16_t dcpu_register_code_read(uint8_t reg_code) {

  uint16_t next_word = 0;
  
  if(reg_code == 0x1E || reg_code == 0x1F || (reg_code >= 0x10 && reg_code <= 0x17))
    next_word = ram_read(internal_dcpu_reg.rPC + DCPU_PC_offset++);

  switch(reg_code) {
    case 0x00:
      DEBUG_STR("DCPU register read", "reg A value");
      return internal_dcpu_reg.rA;
	  
    case 0x01:
      DEBUG_STR("DCPU register read", "reg B value");
      return internal_dcpu_reg.rB;
	  
    case 0x02:
      DEBUG_STR("DCPU register read", "reg C value");
      return internal_dcpu_reg.rC;
	  
    case 0x03:
      DEBUG_STR("DCPU register read", "reg X value");
      return internal_dcpu_reg.rX;
	  
    case 0x04:
      DEBUG_STR("DCPU register read", "reg Y value");
      return internal_dcpu_reg.rY;
	  
    case 0x05:
      DEBUG_STR("DCPU register read", "reg Z value");
      return internal_dcpu_reg.rZ;
	  
    case 0x06:
      DEBUG_STR("DCPU register read", "reg I value");
      return internal_dcpu_reg.rI;
	  
    case 0x07:
      DEBUG_STR("DCPU register read", "reg J value");
      return internal_dcpu_reg.rJ;
	  
	case 0x08:
      DEBUG_STR("DCPU register read", "pointed by reg A value");
      return ram_read(internal_dcpu_reg.rA);
	  
    case 0x09:
      DEBUG_STR("DCPU register read", "pointed by reg B value");
      return ram_read(internal_dcpu_reg.rB);
	  
    case 0x0A:
      DEBUG_STR("DCPU register read", "pointed by reg C value");
      return ram_read(internal_dcpu_reg.rC);
	  
    case 0x0B:
      DEBUG_STR("DCPU register read", "pointed by reg X value");
      return ram_read(internal_dcpu_reg.rX);
	  
    case 0x0C:
      DEBUG_STR("DCPU register read", "pointed by reg Y value");
      return ram_read(internal_dcpu_reg.rY);
	  
    case 0x0D:
      DEBUG_STR("DCPU register read", "pointed by reg Z value");
      return ram_read(internal_dcpu_reg.rZ);
	  
    case 0x0E:
      DEBUG_STR("DCPU register read", "pointed by reg I value");
      return ram_read(internal_dcpu_reg.rI);
	  
    case 0x0F:
      DEBUG_STR("DCPU register read", "pointed by reg J value");
      return ram_read(internal_dcpu_reg.rJ);
    
	case 0x10:
      DEBUG_STR("DCPU register read", "pointed by reg A + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rA);
	
	case 0x11:
      DEBUG_STR("DCPU register read", "pointed by reg B + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rB);
	
	case 0x12:
      DEBUG_STR("DCPU register read", "pointed by reg C + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rC);
	
	case 0x13:
      DEBUG_STR("DCPU register read", "pointed by reg X + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rX);
	
	case 0x14:
      DEBUG_STR("DCPU register read", "pointed by reg Y + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rY);
	
	case 0x15:
      DEBUG_STR("DCPU register read", "pointed by reg Z + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rZ);
	
	case 0x16:
      DEBUG_STR("DCPU register read", "pointed by reg I + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rI);
	
	case 0x17:
      DEBUG_STR("DCPU register read", "pointed by reg J + next_word value");
      return ram_read(next_word + internal_dcpu_reg.rJ);
	
	case 0x18:
      DEBUG_STR("DCPU register read", "POP");
      return ram_read((internal_dcpu_reg.rSP)++);
	  
	case 0x19:
      DEBUG_STR("DCPU register read", "PEEK");
      return ram_read(internal_dcpu_reg.rSP);
	  
	case 0x1A:
      DEBUG_STR("DCPU register read", "PUSH");
      return 0; //ram_read(internal_dcpu_reg.rSP - --DCPU_offset);
	  // Read with a PUSH is non-sense !
	  
	case 0x1B:
      DEBUG_STR("DCPU register read", "SP");
      return internal_dcpu_reg.rSP;
	  
	case 0x1C:
      DEBUG_STR("DCPU register read", "PC");
      return internal_dcpu_reg.rPC + DCPU_PC_offset; // Todo check if to add offset or not
	  
	case 0x1D:
      DEBUG_STR("DCPU register read", "O");
      return internal_dcpu_reg.rO;
	  
	case 0x1E:
	  DEBUG_STR("DCPU register read", "pointed by next_word");
      return ram_read(next_word);
	  
	case 0x1F:
	  DEBUG_STR("DCPU register read", "next_word as literral");
      return next_word;
	  
	default:
	  DEBUG_STR("DCPU register read", "litteral value");
      return reg_code - 0x20; // If reg_code in range 0x20 ~ 0xFF, return associated litteral (0x00 ~ 0x1F)
  }

}

void dcpu_register_code_write(uint8_t reg_code, uint16_t data) {

  uint16_t next_word = 0;
  
  if((reg_code >= 0x10 && reg_code <= 0x17) || reg_code == 0x1E) {
    next_word = ram_read(internal_dcpu_reg.rPC);
  }

  switch(reg_code) {
    case 0x00:
	  DEBUG_STR("DCPU register write", "reg A value");
	  internal_dcpu_reg.rA = data;
	  break;
	  
    case 0x01:
	  DEBUG_STR("DCPU register write", "reg B value");
	  internal_dcpu_reg.rB = data;
	  break;
	  
    case 0x02:
	  DEBUG_STR("DCPU register write", "reg C value");
	  internal_dcpu_reg.rC = data;
	  break;
	  
    case 0x03:
	  DEBUG_STR("DCPU register write", "reg X value");
	  internal_dcpu_reg.rX = data;
	  break;
	  
    case 0x04:
	  DEBUG_STR("DCPU register write", "reg Y value");
	  internal_dcpu_reg.rY = data;
	  break;
	  
    case 0x05:
	  DEBUG_STR("DCPU register write", "reg Z value");
	  internal_dcpu_reg.rZ = data;
	  break;
	  
    case 0x06:
	  DEBUG_STR("DCPU register write", "reg I value");
	  internal_dcpu_reg.rI = data;
	  break;
	  
    case 0x07:
	  DEBUG_STR("DCPU register write", "reg J value");
	  internal_dcpu_reg.rJ = data;
	  break;
	  
	case 0x08:
	  DEBUG_STR("DCPU register write", "pointed by reg A");
	  ram_write(internal_dcpu_reg.rA, data);
	  break;
	  
    case 0x09:
	  DEBUG_STR("DCPU register write", "pointed by reg B");
	  ram_write(internal_dcpu_reg.rB, data);
	  break;
	  
    case 0x0A:
	  DEBUG_STR("DCPU register write", "pointed by reg C");
	  ram_write(internal_dcpu_reg.rC, data);
	  break;
	  
    case 0x0B:
	  DEBUG_STR("DCPU register write", "pointed by reg X");
	  ram_write(internal_dcpu_reg.rX, data);
	  break;
	  
    case 0x0C:
	  DEBUG_STR("DCPU register write", "pointed by reg Y");
	  ram_write(internal_dcpu_reg.rY, data);
	  break;
	  
    case 0x0D:
	  DEBUG_STR("DCPU register write", "pointed by reg Z");
	  ram_write(internal_dcpu_reg.rZ, data);
	  break;
	  
    case 0x0E:
	  DEBUG_STR("DCPU register write", "pointed by reg I");
	  ram_write(internal_dcpu_reg.rI, data);
	  break;
	  
    case 0x0F:
	  DEBUG_STR("DCPU register write", "pointed by reg J");
	  ram_write(internal_dcpu_reg.rJ, data);
	  break;
    
	case 0x10:
	  DEBUG_STR("DCPU register write", "pointed by reg A + next_word");
	  ram_write(next_word + internal_dcpu_reg.rA, data);
	  break;
	
	case 0x11:
	  DEBUG_STR("DCPU register write", "pointed by reg B + next_word");
	  ram_write(next_word + internal_dcpu_reg.rB, data);
	  break;
	
	case 0x12:
	  DEBUG_STR("DCPU register write", "pointed by reg C + next_word");
	  ram_write(next_word + internal_dcpu_reg.rC, data);
	  break;
	
	case 0x13:
	  DEBUG_STR("DCPU register write", "pointed by reg X + next_word");
	  ram_write(next_word + internal_dcpu_reg.rX, data);
	  break;
	
	case 0x14:
	  DEBUG_STR("DCPU register write", "pointed by reg Y + next_word");
	  ram_write(next_word + internal_dcpu_reg.rY, data);
	  break;
	
	case 0x15:
	  DEBUG_STR("DCPU register write", "pointed by reg Z + next_word");
	  ram_write(next_word + internal_dcpu_reg.rZ, data);
	  break;
	
	case 0x16:
	  DEBUG_STR("DCPU register write", "pointed by reg I + next_word");
	  ram_write(next_word + internal_dcpu_reg.rI, data);
	  break;
	
	case 0x17:
	  DEBUG_STR("DCPU register write", "pointed by reg J + next_word");
	  ram_write(next_word + internal_dcpu_reg.rJ, data);
	  break;
	
	case 0x18:
	  DEBUG_STR("DCPU register write", "POP");
      //ram_write((internal_dcpu_reg.rSP)++, data); 
	  // Write to POP is non-sense !
	  break;
	  
	case 0x19:
	  DEBUG_STR("DCPU register write", "PEEK");
      ram_write(internal_dcpu_reg.rSP, data);
	  break;
	  
	case 0x1A:
	  DEBUG_STR("DCPU register write", "PUSH");
      ram_write(--(internal_dcpu_reg.rSP), data);
	  break;
	  
	case 0x1B:
	  DEBUG_STR("DCPU register write", "SP");
      internal_dcpu_reg.rSP = data;
	  break;
	  
	case 0x1C:
	  DEBUG_STR("DCPU register write", "PC");
      internal_dcpu_reg.rPC = data;
	  DCPU_PC_offset = 0;
	  break;
	  
	case 0x1D:
	  DEBUG_STR("DCPU register write", "O");
      internal_dcpu_reg.rO = data;
	  break;
	  
	case 0x1E:
	  DEBUG_STR("DCPU register write", "pointed by next_word");
      ram_write(next_word, data);
	  break;
	  
	default:
	  DEBUG_STR("DCPU register write", "litteral value (ignored)");
      // For other reg_code (0x20 ~ 0xFF = litteral) : fail silently
	  break;
  }
  
}