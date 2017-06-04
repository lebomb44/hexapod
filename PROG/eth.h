#ifndef _ETH_
#define _ETH_

# include "global.h"

# define SYNC_ETH "GET /"
# define TAILLE_SYNC_ETH 5
# define HTML_PAGE "<html><head><title>LOST</title></head><body><center><input type=\"button\" value=\"HAUT\" onClick=\"self.location.href='w'\"><br><input type=\"button\" value=\"GAUCHE\" onClick=\"self.location.href='x'\"><input type=\"button\" value=\"DROITE\" onClick=\"self.location.href='c'\"><br><input type=\"button\" value=\"BAS\" onClick=\"self.location.href='v'\"><br></center></body></html>\n"

void init_eth(void);
void send_eth(u08*);

extern volatile u08 cmd_eth;

#endif
