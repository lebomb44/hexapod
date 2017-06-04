/****************************************************************************
 Titre	:   Gestion du module ethernet EZL-50A
 Auteur:    LeBomb
 Date:      09/10/2007
 Software:  WinAVR
 Cible:     Microcontroleur Atmel AT90S4414/8515/Mega

 DESCRIPTION
       Permet de piloter le modules ethernet EZL-50A.
 UTILISATION
       Liste des fonctions utiles dans eth.h.
*****************************************************************************/

# include "global.h"
# include "eth.h"
# include "sequences.h"

volatile u08 index_eth=0;
volatile u08 cmd_eth=0;

void init_eth(void)
{
  cbi(DDRD,2);
  sbi(PORTD,2);
  // Initialisation de la transmission
  UBRR1H=0x00;    // Baud Rate
  UBRR1L=0x08;    // Baud Rate 57600 bps
  //sbi(UCSR1B,2);  // Mode X bits
  sbi(UCSR1B,3);  // Autorise transmition
  sbi(UCSR1B,4);  // Autorise reception
  //UCSR1C=0x86;    // Mode 9 bits

  index_eth=0;
  cmd_eth = 0;
  sbi(UCSR1B,7);  // Autorisation de l'Interruption Receiver Enable
  sbi(PORTD,2);
}

void send_eth(u08* texte)
{
  while((*texte))
  {
    while(!bit_is_set(UCSR1A,5));
    UDR1=*texte;
    while(!bit_is_set(UCSR0A,5));
    UDR0=*texte;
    texte++;
  }
}

// Fonction d'interruption
ISR(USART1_RX_vect)
{
  u08 temp=0;
  temp=UDR1;
  if(index_eth==TAILLE_SYNC_ETH)
  {
    cmd_eth=temp;
    index_eth=0;
  }
  else
  {
    if((temp==SYNC_ETH[index_eth]) && (index_eth<TAILLE_SYNC_ETH))
    {
      index_eth++;
    }
    else
    {
      index_eth=0;
    }
  }
}
