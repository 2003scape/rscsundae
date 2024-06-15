#ifndef SHOP_H
#define SHOP_H
#include "config/config.h"
#include "entity.h"

void shop_sell(struct shop_config *, struct player *, uint16_t);
void shop_buy(struct shop_config *, struct player *, uint16_t);
#endif
