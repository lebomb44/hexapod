#ifndef _TIMER_
#define _TIMER_

# include "global.h"
# include <avr/eeprom.h>

# define NB_SERVO 20
# define NB_PATTES 6
# define NB_SERVO_PAR_PATTE 3

# define PERIODE_1MS 400
# define REPOS_SERVO 1000
# define MIN_SERVO 0
# define MAX_SERVO 2000

typedef struct Servo
{
  u08* port;
  u08 bit;
  u16 repos;
  u16 min;
  u16 max;
  u16 value;
  u16 next_value;
  s16 step;
  u16 in_seq;
  u16 out_seq;
} Servo;

typedef struct ServoEE
{
  u16 repos;
  u16 min;
  u16 max;
  u16 not_used;
} ServoEE;

u08 p2s(u08, u08);
u08 s2p(u08);
u08 segPatte(u08);
void init_timer(void);
void jump_sequence(u08, u16);
void jump_sequences(u16);
u08 end_sequence(void);
void wait_synchro(void);
void force_servo(u08, u16, u16);
void force_synchro_on_servo(u08);
u08 addMov_rel(u08, s16, u16);
u08 addMov_abs(u08, u16, u16);
u08 addMovPatte_abs(u08, u16, u16, u16, u08);
# define seq1Step(pas, nb_pas, horizon) { seq1Step_(pas, sizeof(pas)/(NB_PATTES*NB_SERVO_PAR_PATTE*sizeof(u16)), nb_pas, horizon); }
void seq1Step_(const u16[][NB_PATTES][NB_SERVO_PAR_PATTE], u16, u16*, u16);

extern volatile Servo servo[NB_SERVO];

#endif
