#ifndef TRADE_H
#define TRADE_H
#include "entity.h"

void player_trade_offer(struct player *, uint16_t, uint32_t);
void player_trade_request(struct player *, uint16_t);
void player_trade_decline(struct player *);
#endif
