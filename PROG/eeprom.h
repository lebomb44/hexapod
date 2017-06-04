#ifndef _EEPROM_
#define _EEPROM_

# include "global.h"

# define EERIE 3
# define EEMWE 2
# define EEWE 1
# define EERE 0

u08 read_eeprom(u16);
void write_eeprom(u16,u08);

#endif
