#ifndef LOOP_H
#define LOOP_H
#include "server.h"

int loop_start(struct server *, int);
int loop_add_player(struct player *);
#endif
