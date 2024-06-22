#include <assert.h>
#include <inttypes.h>
#include "config/config.h"
#include "entity.h"
#include "server.h"

/* corresponds to runescript give() */
void
player_inv_give(struct player *p, struct item_config *item, uint32_t count)
{
	assert(p != NULL);
	assert(item != NULL);
	if (item->weight == 0) {
		/* if weightless, find an existing stack */
		for (int i = 0; i < p->inv_count; ++i) {
			if (p->inventory[i].id != item->id) {
				continue;
			}
			uint32_t new_stack = p->inventory[i].stack + count;
			if (new_stack > MAX_STACK_SIZE ||
			    new_stack < p->inventory[i].stack) {
				p->inventory[i].stack = MAX_STACK_SIZE;
			} else {
				p->inventory[i].stack = new_stack;
			}
			player_send_inv_slot(p, i);
			return;
		}
		if (p->inv_count >= MAX_INV_SIZE) {
			server_add_temp_item(p,
			    p->mob.x, p->mob.y, item->id, count);
			return;
		}
		p->inventory[p->inv_count].id = item->id;
		p->inventory[p->inv_count].stack = count;
		p->inventory[p->inv_count++].worn = false;
		player_send_inv_slot(p, p->inv_count - 1);
		return;
	}
	for (unsigned i = 0; i < count; ++i) {
		if (p->inv_count < MAX_INV_SIZE) {
			p->inventory[p->inv_count].id = item->id;
			p->inventory[p->inv_count].stack = 1;
			p->inventory[p->inv_count++].worn = false;
			player_send_inv_slot(p, p->inv_count - 1);
		} else {
			server_add_temp_item(p,
			    p->mob.x, p->mob.y, item->id, 1);
		}
	}
}

/* corresponds to runescript remove() */
void
player_inv_remove(struct player *p, struct item_config *item, uint32_t count)
{
	uint32_t removed = 0;

	assert(p != NULL);
	assert(item != NULL);

	if (item->weight == 0) {
		for (int i = 0; i < p->inv_count; ++i) {
			if (p->inventory[i].id != item->id) {
				continue;
			}
			if (count < p->inventory[i].stack) {
				p->inventory[i].stack -= count;
				player_send_inv_slot(p, i);
				return;
			}
			count = 1;
			break;
		}
	}
	for (int i = 0; i < p->inv_count && removed < count; ++i) {
		if (p->inventory[i].id != item->id) {
			continue;
		}
		bool was_worn = p->inventory[i].worn;
		p->inv_count--;
		for (int j = i; j < p->inv_count; ++j) {
			p->inventory[j].id = p->inventory[j + 1].id;
			p->inventory[j].stack = p->inventory[j + 1].stack;
			p->inventory[j].worn = p->inventory[j + 1].worn;
		}
		removed++;
		player_send_inv_remove(p, i);
		if (was_worn) {
			player_recalculate_equip(p);
		}
		i = 0;
	}
}

/* corresponds to runescript ifheld() */
bool
player_inv_held(struct player *p, struct item_config *item, uint32_t count)
{
	uint32_t found = 0;

	assert(p != NULL);
	assert(item != NULL);

	for (int i = 0; i < p->inv_count; ++i) {
		if (p->inventory[i].id == item->id) {
			if (item->weight == 0) {
				return p->inventory[i].stack >= count;
			}
			found++;
		}
	}
	return found >= count;
}

/* corresponds to runescript ifworn() */
bool
player_inv_worn(struct player *p, struct item_config *item)
{
	assert(p != NULL);
	assert(item != NULL);

	for (int i = 0; i < p->inv_count; ++i) {
		if (p->inventory[i].id != item->id) {
			continue;
		}
		return p->inventory[i].worn;
	}
	return false;
}

/* corresponds to runescript delinv() */
int
player_inv_clear(struct player *p)
{
	assert(p != NULL);

	p->inv_count = 0;
	p->inv_changed = true;
	return 0;
}
