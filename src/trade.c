#include <stdio.h>
#include <stdlib.h>
#include "trade.h"
#include "inventory.h"
#include "server.h"
#include "utility.h"

void
player_trade_request(struct player *p, uint16_t id)
{
	struct player *target;
	char name[32], message[64];

	if (id >= MAXPLAYERS) {
		return;
	}
	target = p->mob.server->players[id];
	if (target == NULL || player_is_blocked(target,
	    p->name, target->block_trade)) {
		return;
	}
	/* XXX range needs verifying */
	if (abs(p->mob.x - (int)target->mob.x) >= 4 ||
	    abs(p->mob.y - (int)target->mob.y) >= 4) {
		player_send_message(p, "I'm not near enough");
		return;
	}
	p->trading_player = (int16_t)id;
	if (target->trading_player != p->mob.id) {
		player_send_message(p, "Sending trade request");

		mod37_namedec(p->name, name);
		(void)snprintf(message, sizeof(message),
		    "%s wishes to trade with you.", name);
		player_send_message(target, message);
	} else {
		player_send_trade_open(p);
		player_send_trade_open(target);
	}
}

void
player_trade_offer(struct player *p, uint16_t id, uint32_t amount)
{
	struct item_config *config;
	struct player *partner;
	struct invitem item = {0};

	if (p->trading_player == -1 || !p->ui_trade_open) {
		return;
	}
	partner = p->mob.server->players[p->trading_player];
	config = server_item_config_by_id(id);
	if (partner == NULL || config == NULL) {
		return;
	}
	if (config->weight == 0) {
		for (int i = 0; i < p->offer_count; ++i) {
			if (p->trade_offer[i].id != id) {
				continue;
			}
			uint32_t stack = p->trade_offer[i].stack;
			if ((MAX_STACK_SIZE - stack) < amount) {
				stack = MAX_STACK_SIZE;
			} else {
				stack = stack + amount;
			}
			if (!player_inv_held(p, config, stack)) {
				return;
			}
			p->trade_offer[i].stack = stack;
			break;
		}
	}
	if (!player_inv_held(p, config, amount)) {
		return;
	}
	if (config->weight != 0) {
		item.id = id;
		item.stack = 1;
		for (uint32_t i = 0; i < amount; ++i) {
			if (p->offer_count >= MAX_OFFER_SIZE) {
				break;
			}
			p->trade_offer[p->offer_count++] = item;
		}
	} else {
		if (p->offer_count >= MAX_OFFER_SIZE) {
			return;
		}
		item.id = id;
		item.stack = amount;
		p->trade_offer[p->offer_count++] = item;
	}
}

void
player_trade_decline(struct player *p)
{
	struct player *partner;

	if (p->trading_player == -1 || !p->ui_trade_open) {
		return;
	}
	partner = p->mob.server->players[p->trading_player];
	if (partner == NULL) {
		return;
	}
	player_send_close_trade(partner);
	player_send_message(partner, "Other player has declined trade");
	player_close_ui(partner);
	player_close_ui(p);
}
