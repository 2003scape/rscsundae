#ifndef SHOP_H
#define SHOP_H
#include "config/config.h"
#include "entity.h"

int shop_price_modifier(struct shop_config *, struct shop_item *);
void shop_sell(struct shop_config *, struct player *, uint16_t);
void shop_buy(struct shop_config *, struct player *, uint16_t);
#endif
