#ifndef _BLUETOOTH_
#define _BLUETOOTH_

# include "global.h"

void init_blue(void);

void uart_putc(u08);
void uart_putd(u08, u16);
void uart_puts(u08*);
void uart_puts_P(const u08*);

extern volatile u08 cmd_blue;

#endif
