/****************************************************************************
 Titre	:   Gestion radio
 Auteur:    LeBomb
 Date:      13/09/2006
 Software:  WinAVR
 Cible:     Microcontroleur Atmel AT90S4414/8515/Mega

 DESCRIPTION
       Permet de piloter les modules radio.
	RX et TX sont liés par radio.
 UTILISATION
       Liste des fonctions utiles dans radio.h.
*****************************************************************************/

# include "eeprom.h"
# include "global.h"
# include "radio.h"

volatile u08 NBRE_EMISSIONS_RADIO=1;
// Variables de reception
volatile u08 buf_radio[TAILLE_RADIO]={0};
volatile u08 TRASH_radio=0;
volatile u08 chk_sum_radio=0;
volatile u08 curseur_radio=0;

void init_radio(void)
{
  u16 i=0;
  cbi(UCSR1B,7);  // Desactive l'Interruption Receiver Enable
  RADIO_DDR;
  // Initialisation de la transmission
  UBRR1H=0x00;    // Baud Rate
  UBRR1L=0xCF;    // Baud Rate 2400 bps
  sbi(UCSR1B,2);  // Mode X bits
  //sbi(UCSR1B,3);  // Autorise transmition
  sbi(UCSR1B,4);  // Autorise reception
  UCSR1C=0x86;    // Mode 9 bits

  TRASH_radio=0;
  chk_sum_radio=0;
  curseur_radio=0;
  for(i=0; i<TAILLE_RADIO ;i++)
  {
    buf_radio[i]=0;
  }
  NBRE_EMISSIONS_RADIO = read_eeprom(ADR_RADIO+1);
  sbi(UCSR1B,7);  // Autorisation de l'Interruption Receiver Enable
}

//	|	Synchro		|	Adr Recepteur	|	No Commande	|	Nbre d'octets	|	Octet1		|	Octet2		|	Ch Sum	|
void envoi_radio(u08 adr_recepteur, u08 no_command, u08 nbr_data, u08 *data)
{
  u08 h=0;
  u08 i=0;
  u08 j=0;
  u08 temp_UDR=0;
  if(nbr_data>TAILLE_RADIO-4)
  {
    return;
  }
  sbi(UCSR1B,3); // Autorise transmition
  for(h=0;h<NBRE_EMISSIONS_RADIO;h++)
  {
    temp_UDR=0;
    for(i=0;i<(nbr_data+5);i++)
    {
      while(!bit_is_set(UCSR1A,5));
      if(i==0)
      { cbi(UCSR1B,0); } // 9° bit a 0
      else
      { sbi(UCSR1B,0); } // 9° bit a 1

      if(i==0) // Envoi de la synchronisation 10101010 pour minimiser le taux d'erreur binaire et synchroniser l'uart du recepteur
      { UDR1=0xAA; }
      else if(i==1)                // Envoi de l'adresse du recepteur concerné
      { UDR1=adr_recepteur; }
      else if(i==2)                // Envoi du numero de commande
      { UDR1=no_command; }
      else if(i==3)                // Envoi du nombre de data utiles dans la trame
      { UDR1=nbr_data; }
      else if(i>3 && i<nbr_data+4) // Envoi data
      { UDR1=*(data+i-4); }
      else if(i==(nbr_data+4))     // Envoi du check sum
      {
        for(j=0;j<nbr_data;j++) { temp_UDR= (temp_UDR+*(data+j)); }
        temp_UDR = adr_recepteur + no_command + nbr_data + temp_UDR;
        UDR1 = temp_UDR;
      }
    }
  }
  while(!bit_is_set(UCSR1A,5));
  cbi(UCSR1B,3); // Desactive transmition
}

// Fonction d'interruption
ISR(USART1_RX_vect)
{
  u16 i=0;
  if(bit_is_clear(UCSR1B,1) && bit_is_clear(UCSR1A,4))
  {
    buf_radio[0]=UDR1;
    curseur_radio=0;buf_radio[3]=TAILLE_RADIO-4; buf_radio[2]==0x00; buf_radio[1]==0x00;
  }
  else 
  {
    if(bit_is_set(UCSR1B,1) && bit_is_clear(UCSR1A,4) && bit_is_clear(UCSR1A,3) && (buf_radio[0]==0xAA))
    {
      curseur_radio++;
      buf_radio[curseur_radio]=UDR1;
    }
    else
    {
      TRASH_radio=UDR1; curseur_radio=0;
      buf_radio[3]=TAILLE_RADIO-4; buf_radio[2]==0x00; buf_radio[1]==0x00; buf_radio[0]==0x00;
    }
  }

  if((curseur_radio>(buf_radio[3]+3)) || (curseur_radio>TAILLE_RADIO-4))
  {
    if((buf_radio[0]==0xAA) && (curseur_radio<=TAILLE_RADIO-4))
    {
      chk_sum_radio=0;
      for(i=1;i<(buf_radio[3]+4);i++)
      { chk_sum_radio=(u08) (chk_sum_radio+buf_radio[i]); }
      if(chk_sum_radio==buf_radio[buf_radio[3]+4])
      {		// liste des commandes
        //addTC((u08*) &buf_radio[1]);
      }
    }
    curseur_radio=0;
    buf_radio[3]=TAILLE_RADIO-4; buf_radio[2]=0x00; buf_radio[1]=0x00; buf_radio[0]=0x00;
  }
}
