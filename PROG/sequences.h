#ifndef _SEQUENCES_
#define _SEQUENCES_

# include "global.h"
# include "timer.h"

# define MAX_CDES 256

# define MODE_DEFAULT 0
# define MODE_REGLAGE 0
# define MODE_PILOT   1
# define NB_MODE      2

void init_sequences(void);
void commandes(u08*);
void sequences(void);

#endif
