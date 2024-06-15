#include <limits.h>
#include "config/config.h"
#include "entity.h"
#include "inventory.h"
#include "server.h"

#define MAX_SHOP_STACK UINT16_MAX

static int shop_remove(struct shop_config *, struct player *, uint16_t);

void
shop_sell(struct shop_config *shop, struct player *p, uint16_t id)
{
	struct item_config *item;

	item = server_item_config_by_id(id);
	if (p->shop == NULL || item == NULL ||
	    !player_inv_held(p, item, 1)) {
		return;
	}
	for (size_t i = 0; i < shop->item_count; ++i) {
		if (shop->items[i].id == id) {
			if (shop->items[i].quantity == MAX_SHOP_STACK) {
				return;
			}
			player_inv_remove(p, item, 1);
			shop->items[i].quantity++;
			player_send_shop(p, shop->name);
			return;
		}
	}
	if (shop->item_count < MAX_SHOP_ITEMS) {
		player_inv_remove(p, item, 1);
		shop->items[shop->item_count].id = id;
		shop->items[shop->item_count].restock = 0;
		shop->items[shop->item_count++].quantity = 1;
		player_send_shop(p, shop->name);
	}
}

static int
shop_remove(struct shop_config *shop, struct player *p, uint16_t id)
{
	for (size_t i = 0; i < shop->item_count; ++i) {
		if (shop->items[i].id != id) {
			continue;
		}
		if (shop->items[i].quantity == 0) {
			return -1;
		}
		if (shop->items[i].quantity == 1 && !shop->items[i].restock) {
			shop->item_count--;
			for (size_t j = i; j < shop->item_count; ++j) {
				shop->items[j] = shop->items[j + 1];
			}
		} else {
			p->shop->items[i].quantity--;
		}
		player_send_shop(p, shop->name);
		return 0;
	}
	return -1;
}

void
shop_buy(struct shop_config *shop, struct player *p, uint16_t id)
{
	struct item_config *item;

	item = server_item_config_by_id(id);
	if (p->shop == NULL || item == NULL) {
		return;
	}
	if (item->weight == 0) {
		for (size_t i = 0; i < p->inv_count; ++i) {
			if (p->inventory[i].id != id) {
				continue;
			}
			if (shop_remove(shop, p, id) != -1) {
				player_inv_give(p, item, 1);
			}
			return;
		}
	}
	if (p->inv_count == MAX_INV_SIZE) {
		player_send_message(p,
		    "You can't hold the objects you are trying to buy!");
	} else {
		if (shop_remove(shop, p, id) != -1) {
			player_inv_give(p, item, 1);
		}
	}
	return;
}
