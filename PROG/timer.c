/****************************************************************************
 Titre	:   Gestion timer
 Auteur:    LeBomb
 Date:      13/09/2006
 Software:  WinAVR
 Cible:     Microcontroleur Atmel AT90S4414/8515/Mega

 DESCRIPTION
       Routines de gestion des timers
       Scrutation cycles des différentes entrees : capteurs, GPS, RTC, IP sur horloge, IO
 UTILISATION
       Liste des fonctions utiles dans timer.h
*****************************************************************************/

# include "blue.h"
# include "eeprom.h"
# include "global.h"
# include "timer.h"

volatile Servo servo[NB_SERVO];
ServoEE EEMEM servoEE[NB_SERVO] = {
{1900, MIN_SERVO, MAX_SERVO, 0}, {1240, MIN_SERVO, MAX_SERVO, 0}, {1120, MIN_SERVO, MAX_SERVO, 0},
{1740, MIN_SERVO, MAX_SERVO, 0}, {1300, MIN_SERVO, MAX_SERVO, 0}, {1190, MIN_SERVO, MAX_SERVO, 0},
{   0, MIN_SERVO, MAX_SERVO, 0}, { 500, MIN_SERVO, MAX_SERVO, 0}, { 580, MIN_SERVO, MAX_SERVO, 0},
{REPOS_SERVO, MIN_SERVO, MAX_SERVO, 0}, {REPOS_SERVO, MIN_SERVO, MAX_SERVO, 0},
{1900, MIN_SERVO, MAX_SERVO, 0}, {1420, MIN_SERVO, MAX_SERVO, 0}, {1230, MIN_SERVO, MAX_SERVO, 0},
{   0, MIN_SERVO, MAX_SERVO, 0}, { 500, MIN_SERVO, MAX_SERVO, 0}, {1050, MIN_SERVO, MAX_SERVO, 0},
{  30, MIN_SERVO, MAX_SERVO, 0}, { 600, MIN_SERVO, MAX_SERVO, 0}, { 900, MIN_SERVO, MAX_SERVO, 0}};
volatile u08 no_servo = 0;

void init_timer(void)
{
  volatile u08 i=0;
  servo[0].port=(u08*) &PORTF;  servo[0].bit=3;  servo[1].port=(u08*) &PORTF;  servo[1].bit=2;
  servo[2].port=(u08*) &PORTF;  servo[2].bit=1;  servo[3].port=(u08*) &PORTF;  servo[3].bit=0;
  servo[4].port=(u08*) &PORTE;  servo[4].bit=3;  servo[5].port=(u08*) &PORTB;  servo[5].bit=0;
  servo[6].port=(u08*) &PORTB;  servo[6].bit=6;  servo[7].port=(u08*) &PORTB;  servo[7].bit=7;
  servo[8].port=(u08*) &PORTG;  servo[8].bit=3;  servo[9].port=(u08*) &PORTG;  servo[9].bit=4;
  servo[10].port=(u08*) &PORTC; servo[10].bit=0; servo[11].port=(u08*) &PORTC; servo[11].bit=1;
  servo[12].port=(u08*) &PORTC; servo[12].bit=2; servo[13].port=(u08*) &PORTC; servo[13].bit=3;
  servo[14].port=(u08*) &PORTC; servo[14].bit=4; servo[15].port=(u08*) &PORTC; servo[15].bit=5;
  servo[16].port=(u08*) &PORTA; servo[16].bit=3; servo[17].port=(u08*) &PORTA; servo[17].bit=2;
  servo[18].port=(u08*) &PORTA; servo[18].bit=1; servo[19].port=(u08*) &PORTA; servo[19].bit=0;
  PORTA=0; PORTB=0; PORTC=0; PORTD=0; PORTE=0; PORTF=0; PORTG=0;
  for(i=0; i<NB_SERVO; i++)
  {
    if(servo[i].port==(&PORTA))
      { DDRA = DDRA | (1<<servo[i].bit); }
    if(servo[i].port==(&PORTB))
      { DDRB = DDRB | (1<<servo[i].bit); }
    if(servo[i].port==(&PORTC))
      { DDRC = DDRC | (1<<servo[i].bit); }
    if(servo[i].port==(&PORTD))
      { DDRD = DDRD | (1<<servo[i].bit); }
    if(servo[i].port==(&PORTE))
      { DDRE = DDRE | (1<<servo[i].bit); }
    if(servo[i].port==(&PORTF))
      { DDRF = DDRF | (1<<servo[i].bit); }
    if(servo[i].port==(&PORTG))
      { DDRG = DDRG | (1<<servo[i].bit); }
    servo[i].repos=((u16)read_eeprom((u16)((u08*)&servoEE[i].repos))+(((u16)read_eeprom((u16)(((u08*)&servoEE[i].repos)+1)))<<8));
    servo[i].value=U16_MAX-PERIODE_1MS-servo[i].repos;
    servo[i].next_value=servo[i].value;
    servo[i].min=((u16)read_eeprom((u16)((u08*)&servoEE[i].min))+(((u16)read_eeprom((u16)(((u08*)&servoEE[i].min)+1)))<<8));
    servo[i].max=((u16)read_eeprom((u16)((u08*)&servoEE[i].max))+(((u16)read_eeprom((u16)(((u08*)&servoEE[i].max)+1)))<<8));
    servo[i].step=0;
    servo[i].in_seq=0;
    servo[i].out_seq=0;
    cbi(*(servo[i].port),servo[i].bit);
  }
  no_servo=0;
  TCCR1A=0x00;
  TCCR1B=0x00;
  TIMSK=0x00;   // Desactive interrupt
  TIFR=0x04;    // Clear interrupt flags
  TCNT1=0x0000; // Clear counter
  TIMSK=0x04;   // Active interrupt
  TCCR1B=0x02;  // CLK / 8
}

u08 p2s(u08 p, u08 s)
{
  if(p>=NB_PATTES)
  { return 0xFF;}
  if(s>=NB_SERVO_PAR_PATTE)
  { return 0xFF;}
  p=p*3;
  if(p>8)
  { p=p+2; }
  p=p+s;
  return p;
}

u08 s2p(u08 s)
{
  if(s>=NB_SERVO)
  { return 0xFF; }
  if(s<9)
  { return (s/3); }
  if(s>10)
  { return ((s-2)/3); }
  return 0xFF;
}

u08 segPatte(u08 s)
{
  if(s>=NB_SERVO)
  { return 0xFF; }
  u08 p=0;
  p=s2p(s)*3;
  if(p>=NB_PATTES)
  { return 0xFF; }
  if(p<3)
  { return (s-p); }
  return (s-2-p);
}

void jump_sequence(u08 serv, u16 sequence)
{
  if(serv>=NB_SERVO)
  { return; }
  servo[serv].step=0;
  servo[serv].out_seq=sequence;
}

void jump_sequences(u16 sequence)
{
  u08 i=0;
  for(i=0; i<NB_SERVO; i++)
  { jump_sequence(i, sequence); }
}

u08 end_sequence(void)
{
  u08 i=0;
  u08 j=0;
  u08 k=0;
  u08 serv=0;
  j=0;
  for(i=0; i<NB_PATTES; i++)
  {
    for(j=0; j<NB_SERVO_PAR_PATTE; j++)
    {
      serv=p2s(i,j);
      if((servo[serv].in_seq==servo[serv].out_seq) && (servo[serv].step==0))
      {
        k++;
      }
    }
  }
  if(k==(NB_PATTES*NB_SERVO_PAR_PATTE))
  {
    return 1;
  }
  else
  { return 0; }
}

void wait_synchro(void)
{
  u08 i=0;
  u08 j=0;
  if(end_sequence())
  {
    for(i=0; i<NB_PATTES; i++)
    {
      for(j=0; j<NB_SERVO_PAR_PATTE; j++)
      { servo[p2s(i,j)].step=1; } // Sequence suivante
    }
  }
  for(i=0; i<NB_PATTES; i++)
  {
    for(j=0; j<NB_SERVO_PAR_PATTE; j++)
    { servo[p2s(i,j)].in_seq++; } // Sequence suivante
  }
}

void force_servo(u08 serv, u16 value, u16 horizon)
{
  if(serv>=NB_SERVO)
  { return; }
  if(value>servo[serv].max) // Verification de la valeur passee
  { value = servo[serv].max; }
  if(value<servo[serv].min)
  { value = servo[serv].min; }
  //servo[serv].next_value=servo[serv].value; // Mise a jour de la valeur
  servo[serv].value=U16_MAX-PERIODE_1MS-value;
  if(horizon==0) // Rotation immediate
  {
    servo[serv].next_value=servo[serv].value;
    servo[serv].step=1;
  }
  else // Calcul du step
  {
    if(servo[serv].next_value<servo[serv].value)
    {
      servo[serv].step=(servo[serv].value-servo[serv].next_value)/horizon;
      if(servo[serv].step==0)
      { servo[serv].step=1; }
    }
    else
    {
      servo[serv].step=-((servo[serv].next_value-servo[serv].value)/horizon);
      if(servo[serv].step==0)
      { servo[serv].step=-1; }
    }
  }
}

void force_synchro_on_servo(u08 serv)
{
  u08 i=0;
  if(serv>=NB_SERVO)
  { return; }
  if((servo[serv].in_seq==servo[serv].out_seq) && (servo[serv].step==0))
  {
    for(i=0; i<NB_SERVO; i++)
    { force_servo(i, servo[i].value, 0); }
  }
  for(i=0; i<NB_SERVO; i++)
  { servo[i].in_seq++; }
}

u08 addMov_rel(u08 serv, s16 value, u16 horizon)
{
  s32 val=0;
  if(serv>=NB_SERVO)
  { return 0; }
  val=U16_MAX-PERIODE_1MS-servo[serv].value-value;
  if(val> (s32) servo[serv].max)
  { val = (s32) servo[serv].max; }
  if(val< (s32) servo[serv].min)
  { val = (s32) servo[serv].min; }
  return addMov_abs(serv, (u16) val, horizon);
}

u08 addMov_abs(u08 serv, u16 value, u16 horizon)
{
  if(serv>=NB_SERVO)
  { return 0; }
  if((servo[serv].in_seq==servo[serv].out_seq) && (servo[serv].step==0))
  {
    force_servo(serv, value, horizon);
  }
  servo[serv].in_seq++; // Sequence suivante
  return 1;
}

u08 addMovPatte_abs(u08 patte, u16 serv0, u16 serv1, u16 serv2, u08 horizon)
{
  u08 ret=0;
  ret=    addMov_abs(p2s(patte,0), serv0, horizon);
  ret=ret+addMov_abs(p2s(patte,1), serv1, horizon);
  ret=ret+addMov_abs(p2s(patte,2), serv2, horizon);
  return ret;
}

// Un pas
void seq1Step_(const u16 pas[][NB_PATTES][NB_SERVO_PAR_PATTE], u16 nb_step, u16* nb_pas, u16 horizon)
{
  u08 i=0;
  u08 j=0;
  for(i=0; i<nb_step; i++)
  {
    for(j=0; j<NB_PATTES; j++)
    { addMovPatte_abs(j, pgm_read_word(&pas[i][j][0]), pgm_read_word(&pas[i][j][1]), pgm_read_word(&pas[i][j][2]), horizon); }
    wait_synchro();
  }
  if(nb_pas!=0)
  {
    if(end_sequence())
    { if((*nb_pas)>U16_MIN) { (*nb_pas)--; { jump_sequences(0); } } }
  }
}

ISR(TIMER1_OVF_vect)
{
  sbi(*(servo[no_servo].port),servo[no_servo].bit);
  if(no_servo==0)
  { cbi(*(servo[NB_SERVO-1].port),servo[NB_SERVO-1].bit); }
  else
  { cbi(*(servo[no_servo-1].port),servo[no_servo-1].bit); }
  TCNT1=servo[no_servo].next_value;

  if(servo[no_servo].step!=0)
  {
    servo[no_servo].next_value=servo[no_servo].next_value+servo[no_servo].step;
    if(servo[no_servo].step>0)
    { if(servo[no_servo].next_value>servo[no_servo].value) { servo[no_servo].next_value=servo[no_servo].value; } }
    else
    { if(servo[no_servo].next_value<servo[no_servo].value) { servo[no_servo].next_value=servo[no_servo].value; } }

    if(servo[no_servo].value==servo[no_servo].next_value)
    { servo[no_servo].step=0; servo[no_servo].out_seq++; }
  }
  // Servo suivant
  no_servo++;
  if(no_servo>=NB_SERVO)
  { no_servo=0; }
}
