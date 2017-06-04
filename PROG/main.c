# include "blue.h"
# include "delay.h"
# include "eth.h"
# include "global.h"
# include "sequences.h"
# include "timer.h"

int main(void)
{
  init_blue();
  init_eth();
  init_timer();
  init_sequences();
  delay_ms(1000);
  //Sleep mode
  sbi(MCUCR,5);
  uart_puts((u08*) "--- Start : Robot ARAIGNEE ---\n");
  //cmd_blue='z';
  sbi(SREG,7);
  sbi(LEDDDR,LEDBIT);
  while(1)
  {
    LEDTOGGLE;
    commandes((u08*) &cmd_eth);
    commandes((u08*) &cmd_blue);
    sequences();
	sleep(); // Mise en veille. Reveil par une interruption.
  }
  return (0);
}
