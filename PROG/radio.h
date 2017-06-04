#ifndef _RADIO_
#define _RADIO_

# include "global.h"

# define RADIO_ENRXDDR DDRD
# define RADIO_ENRXPORT PORTD
# define RADIO_ENRXPIN PIND
# define RADIO_ENRXBIT 6
# define RADIO_ENRX sbi(RADIO_ENRXPORT,RADIO_ENRXBIT)
# define RADIO_DIRX cbi(RADIO_ENRXPORT,RADIO_ENRXBIT)

# define RADIO_ENTXDDR DDRD
# define RADIO_ENTXPORT PORTD
# define RADIO_ENTXPIN PIND
# define RADIO_ENTXBIT 7
# define RADIO_ENTX sbi(RADIO_ENTXPORT,RADIO_ENTXBIT)
# define RADIO_DITX cbi(RADIO_ENTXPORT,RADIO_ENTXBIT)

# define RADIO_DDR { RADIO_DITX; RADIO_DIRX; \
                     sbi(RADIO_ENTXDDR,RADIO_ENTXBIT); sbi(RADIO_ENRXDDR,RADIO_ENRXBIT); }

# define TAILLE_RADIO (TAILLE_TC+2) // Synchro et Checksum

void envoi_radio(u08, u08, u08, u08*);
void init_radio(void);

#endif
