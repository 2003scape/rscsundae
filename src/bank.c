#include "config/config.h"
#include "entity.h"
#include "inventory.h"
#include "server.h"

static uint32_t player_bank_remove(struct player *, uint16_t, uint32_t);

void
player_deposit(struct player *p, uint16_t id, uint32_t amount)
{
	struct item_config *config;

	config = server_item_config_by_id(id);
	if (!p->ui_bank_open || config == NULL ||
	    !player_inv_held(p, config, amount)) {
		return;
	}
	for (size_t i = 0; i < p->bank_count; ++i) {
		if (p->bank[i].id == id) {
			if ((MAX_STACK_SIZE - p->bank[i].amount) < amount) {
				return;
			}
			player_inv_remove(p, config, amount);
			p->bank[i].amount += amount;
			player_send_show_bank(p);
			return;
		}
	}
	if (p->bank_count < MAX_BANK_SIZE) {
		player_inv_remove(p, config, amount);
		p->bank[p->bank_count].id = id;
		p->bank[p->bank_count++].amount = amount;
		player_send_show_bank(p);
	}
}

static uint32_t
player_bank_remove(struct player *p, uint16_t id, uint32_t amount)
{
	for (size_t i = 0; i < p->bank_count; ++i) {
		if (p->bank[i].id != id) {
			continue;
		}
		if (amount >= p->bank[i].amount) {
			amount = p->bank[i].amount;
			p->bank_count--;
			for (size_t j = i; j < p->bank_count; ++j) {
				p->bank[j] = p->bank[j + 1];
			}
		} else {
			p->bank[i].amount -= amount;
		}
		if (amount > 0) {
			player_send_show_bank(p);
		}
		return amount;
	}
	return 0;
}

void
player_withdraw(struct player *p, uint16_t id, uint32_t amount)
{
	struct item_config *config;
	int needed_space;

	config = server_item_config_by_id(id);
	if (!p->ui_bank_open || config == NULL) {
		return;
	}
	if (config->weight == 0) {
		for (size_t i = 0; i < p->inv_count; ++i) {
			if (p->inventory[i].id != id) {
				continue;
			}
			amount = player_bank_remove(p, id, amount);
			if (amount > 0) {
				player_inv_give(p, config, amount);
			}
			return;
		}
	}
	needed_space = config->weight == 0 ? 1 : amount;
	if ((MAX_INV_SIZE - p->inv_count) < needed_space) {
		player_send_message(p,
		    "You don't have room to hold everything!");
	} else {
		amount = player_bank_remove(p, id, amount);
		if (amount > 0) {
			player_inv_give(p, config, amount);
		}
	}
	return;
}
