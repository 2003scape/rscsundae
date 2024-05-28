#ifndef NETIO_H
#define NETIO_H

#include "entity.h"

int
net_establish_listener(int *, int);

int
net_player_send(struct player *p);

int
net_player_recv(struct player *p);

#endif
