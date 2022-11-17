#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "defines.h"

void unlock_eeprom();
__ramfunc void ProgramOptionByte(uint Address, uchar Data);
__ramfunc void save_word_eeprom(uint *addres, uint data);
__ramfunc void save_byte_eeprom(uchar *addres, uchar data);

#endif