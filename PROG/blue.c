/****************************************************************************
 Titre	:   Gestion de l'interface bluetooth & PC
 Auteur:    LeBomb
 Date:      13/09/2006
 Software:  WinAVR
 Cible:     Microcontroleur Atmel AT90S4414/8515/Mega

 DESCRIPTION
       Permet de piloter le module bluetooth
 UTILISATION
       Liste des fonctions utiles dans blue.h.
*****************************************************************************/

# include "blue.h"
# include "global.h"
# include "sequences.h"
# include "timer.h"

volatile u08 cmd_blue=0;

void init_blue(void)
{
  cbi(DDRE,0);
  sbi(PORTE,0);
  // Initialisation de la transmission
  UBRR0H=0x00;    // Baud Rate
//UBRR0L=0x0C;    // Baud Rate 38000 bps
  UBRR0L=0x08;    // Baud Rate 57600 bps
//sbi(UCSR0B,2);  // Mode X bits
  sbi(UCSR0B,3);  // Autorise transmition
  sbi(UCSR0B,4);  // Autorise reception
//UCSR0C=0x86;    // Mode 9 bits

  cmd_blue = 0;
  sbi(UCSR0B,7);  // Autorisation de l'Interruption Receiver Enable
  sbi(PORTE,0);
}

// Fonction d'interruption
ISR(USART0_RX_vect)
{
  cmd_blue=UDR0;
}

void uart_putc(u08 data)
{
//cbi(UCSR0B,0);
  while(!bit_is_set(UCSR0A,5));
  UDR0=data;
}

void uart_putd(u08 type, u16 data)
{
  static const unsigned char hex[] = "0123456789ABCDEF";
  u08 i=8;
  u16 div_val=0;
  switch (type)
  {
    case 'b':
      i=8;
      do { i--; if(bit_is_set(data,i)) {uart_putc('1');} else {uart_putc('0');} } while(i);
      break;
    case 'c':
      uart_putc(data);
      break;
    case 'd':
      div_val = 10000;
      while (div_val > 1 && div_val > data) { div_val /= DEC; }
      do { uart_putc(hex[data / div_val]); data %= div_val; div_val /= DEC; }
      while (div_val);
      break;
    case 'x':
      div_val = 0x10;
      do { uart_putc(hex[data / div_val]); data %= div_val; div_val /= HEX; }
      while (div_val);
      break;
    case 'm':
      div_val = 0x1000;
      do { uart_putc(hex[data / div_val]); data %= div_val; div_val /= HEX; }
      while (div_val);
      break;
    default:
      uart_putc(type);
      break;
  }
}

void uart_puts(u08* texte)
{
  while(*texte)
  { uart_putc(*texte); texte++; }
}

void uart_puts_P(const u08* texte)
{
  while(pgm_read_byte(texte))
  { uart_putc(pgm_read_byte(texte)); texte++; }
}
