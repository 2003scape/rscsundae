#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "trade.h"
#include "inventory.h"
#include "server.h"
#include "utility.h"

static void player_trade_finalize(struct player *);

void
player_trade_request(struct player *p, struct player *target)
{
	char name[32], message[64];

	assert(p != NULL);
	assert(target != NULL);

	if (player_is_blocked(target, p->name, target->block_trade)) {
		return;
	}
	/* XXX range needs verifying */
	if (abs(p->mob.x - (int)target->mob.x) >= 4 ||
	    abs(p->mob.y - (int)target->mob.y) >= 4) {
		player_send_message(p, "I'm not near enough");
		return;
	}
	p->trading_player = (int16_t)target->mob.id;
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
	/*
	 * XXX: allow this for now, 110 protocol has no way to notify
	 * player their own stuff was removed from a trade
	 */
	/*if (config->quest_item) {
		return;
	}*/
	if (p->trade_state == TRADE_STATE_CONFIRMED ||
	    partner->trade_state == TRADE_STATE_CONFIRMED) {
		return;
	}
	/* offer is about to change so unaccept the trade */
	if (p->trade_state != TRADE_STATE_NONE) {
		p->trade_state = TRADE_STATE_NONE;
		player_send_trade_state(p);
		player_send_trade_state_remote(partner);
	}
	if (partner->trade_state != TRADE_STATE_NONE) {
		partner->trade_state = TRADE_STATE_NONE;
		player_send_trade_state(partner);
		player_send_trade_state_remote(p);
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
			if (!player_inv_held_id(p, id, stack)) {
				return;
			}
			p->trade_offer[i].stack = stack;
			break;
		}
	}
	if (!player_inv_held_id(p, id, amount)) {
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
player_trade_accept(struct player *p)
{
	struct player *partner;

	if (p->trading_player == -1 || !p->ui_trade_open) {
		return;
	}

	partner = p->mob.server->players[p->trading_player];
	if (partner == NULL) {
		return;
	}
	if (p->trade_state != TRADE_STATE_NONE) {
		return;
	}
	p->trade_state = TRADE_STATE_ACCEPTED;
	if (partner->trade_state == TRADE_STATE_ACCEPTED) {
		/*
		 * work around a bug in the 110 client where it doesn't
		 * display the actual final trade offer it gets sent
		 */
		player_send_partner_trade_offer(p);
		player_send_partner_trade_offer(partner);
		player_send_trade_confirm(p);
		player_send_trade_confirm(partner);
	} else {
		player_send_trade_state(p);
		player_send_trade_state_remote(partner);
	}
}

void
player_trade_confirm(struct player *p)
{
	struct player *partner;

	if (p->trading_player == -1 || !p->ui_trade_open) {
		return;
	}

	partner = p->mob.server->players[p->trading_player];
	if (partner == NULL) {
		return;
	}
	if (p->trade_state != TRADE_STATE_ACCEPTED) {
		return;
	}
	p->trade_state = TRADE_STATE_CONFIRMED;
	if (partner->trade_state == TRADE_STATE_CONFIRMED) {
		player_trade_finalize(p);
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
		player_close_ui(p);
		return;
	}
	player_send_message(partner, "Other player has declined trade");
	player_close_ui(partner);
	player_close_ui(p);
}

static void
player_trade_finalize(struct player *p)
{
	struct player *partner;
	struct item_config *config;

	if (p->trading_player == -1 || !p->ui_trade_open) {
		return;
	}
	partner = p->mob.server->players[p->trading_player];
	if (partner == NULL) {
		player_close_ui(p);
		return;
	}

	for (int i = 0; i < p->offer_count; ++i) {
		player_inv_remove_id(p,
		    p->trade_offer[i].id,
		    p->trade_offer[i].stack);
	}
	for (int i = 0; i < partner->offer_count; ++i) {
		player_inv_remove_id(partner,
		    partner->trade_offer[i].id,
		    partner->trade_offer[i].stack);
	}
	for (int i = 0; i < p->offer_count; ++i) {
		config = server_item_config_by_id(p->trade_offer[i].id);
		player_inv_give(partner, config, p->trade_offer[i].stack);
	}
	for (int i = 0; i < partner->offer_count; ++i) {
		config = server_item_config_by_id(partner->trade_offer[i].id);
		player_inv_give(p, config, partner->trade_offer[i].stack);
	}

	player_close_ui(p);
	player_close_ui(partner);
	player_send_message(p, "Trade completed successfully");
	player_send_message(partner, "Trade completed successfully");
}
