/****************************************************************************
 Titre	:   Parametres globaux
 Auteur:    LeBomb
 Date:      02/09/2004
 Software:  WinAVR
 Cible:     Microcontroleur Atmel AT90S4414/8515/Mega

 DESCRIPTION
       Definit et declare tous les parametres globaux relatifs a l'application
 UTILISATION
       Definir la valeur du QUARTZ
	   Choix du microcontroleur utilise:
		   librairy <avr/iomXX.h>
		   dans le makefile MCU = atmegaXX
*****************************************************************************/

#ifndef _GLOBAL_
#define _GLOBAL_

typedef unsigned char  u08;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef signed char  s08;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

#define U08_MIN 0
#define U08_MAX 255
#define U16_MIN 0
#define U16_MAX 65535

#define QUARTZ 8000 // frequence du quartz en KHz, sert pour les fonctions d'attente

#define GCC33

// includes generaux
#include <avr/io.h>
#include <avr/iom128.h>  // definition du processeur
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <avr/pgmspace.h>
//#include <stdlib.h>

#define extr16_08_0(i) (*(char *)(&i))
#define extr16_08_1(i) (*((char *)(&i)+1))

#define extr32_16_0(i) (*(int *)(&i)) // poids faible
#define extr32_16_1(i) (* (((int *)(&i)) +1)) //poids fort
#define extr32_16_23(i) (*((int *)((char *)(&i)+1))) // les 2 octets du milieu

#define extr32_08_0(i) (*(char *)(&i))
#define extr32_08_1(i) (*((char *)(&i)+1))
#define extr32_08_2(i) (*((char *)(&i)+2))
#define extr32_08_3(i) (*((char *)(&i)+3))

# define sbi(reg,bit) { reg = reg |   (1<<bit) ; }
# define cbi(reg,bit) { reg = reg & (~(1<<bit)); }
# define nop() __asm__ __volatile__ ("NOP");
# define rien() __asm__ __volatile__ (" ");
# define sleep() __asm__ __volatile__ ("SLEEP");

#define BIN 2
#define CHAR 8
#define DEC 10
#define HEX 16
#define MOT 32

// Adresses EEPROM
# define ADR_DEVICE   0
# define ADR_TIMER    (ADR_DEVICE+8)
# define ADR_RADIO    (ADR_TIMER+8)
# define ADR_BLUE     (ADR_RADIO+8)
# define ADR_SERVO    (ADR_BLUE+8)
# define EEPROM_SERVO 8

# define LEDDDR DDRF
# define LEDPORT PORTF
# define LEDPIN PINF
# define LEDBIT 4
# define LEDON sbi(LEDPORT,LEDBIT)
# define LEDOFF cbi(LEDPORT,LEDBIT)
# define LEDTOGGLE {if(bit_is_set(LEDPIN,LEDBIT)) { cbi(LEDPORT,LEDBIT); } else { sbi(LEDPORT,LEDBIT); }}

# define TAILLE_TC 7 // Adr_recepteur | Cmd | Nb_data | data

#endif
