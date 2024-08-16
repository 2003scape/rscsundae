#ifndef TRADE_H
#define TRADE_H
#include "entity.h"

void player_trade_offer(struct player *, uint16_t, uint32_t);
void player_trade_request(struct player *, struct player *);
void player_trade_accept(struct player *);
void player_trade_decline(struct player *);
void player_trade_confirm(struct player *);
#endif
