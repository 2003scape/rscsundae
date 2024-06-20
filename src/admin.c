#include <stdio.h>
#include <string.h>
#include "entity.h"
#include "inventory.h"
#include "server.h"
#include "stat.h"

void
player_parse_admin_command(struct player *p, char *str)
{
	const char *cmd;

	printf("admin command: %s\n", str);
	cmd = strtok(str, " ");
	if (strcmp(cmd, "give") == 0) {
		struct item_config *item_config;
		char *item_str;
		char *amount_str;
		char mes[64];
		int amount;

		item_str = strtok(NULL, " ");
		amount_str = strtok(NULL, " ");

		if (item_str == NULL || amount_str == NULL) {
			player_send_message(p, "Usage: give itemname amount");
			return;
		}

		for (size_t i = 0; item_str[i] != '\0'; ++i) {
			if (item_str[i] == '_') {
				item_str[i] = ' ';
			}
		}
		item_config = server_find_item_config(item_str);
		if (item_config == NULL) {
			(void)snprintf(mes, sizeof(mes),
			    "No such item: %s", item_str);
			player_send_message(p, mes);
			return;
		}
		amount = strtol(amount_str, NULL, 10);
		if (item_config->weight > 0 &&
		    (MAX_INV_SIZE - p->inv_count) < amount) {
			player_send_message(p, "Not enough space!");
			return;
		}
		player_inv_give(p, item_config, amount);
	} else if (strcmp(cmd, "tele") == 0) {
		char *x_str;
		char *y_str;

		x_str = strtok(NULL, " ");
		y_str = strtok(NULL, " ");

		if (x_str == NULL || y_str == NULL) {
			player_send_message(p,
			    "Usage: tele relative_x relative_y");
			return;
		}

		/* jagex coordinate syntax not yet supported */
		p->mob.x += strtol(x_str, NULL, 10);
		p->mob.y += strtol(y_str, NULL, 10);
		p->teleported = true;
	} else if (strcmp(cmd, "ftele") == 0) {
		char *x_str;
		char *y_str;

		x_str = strtok(NULL, " ");
		y_str = strtok(NULL, " ");

		if (x_str == NULL || y_str == NULL) {
			player_send_message(p, "Usage: ftele x y");
			return;
		}

		/* jagex coordinate syntax not yet supported */
		p->mob.x = strtol(x_str, NULL, 10);
		p->mob.y = strtol(y_str, NULL, 10);
		p->teleported = true;
	} else if (strcmp(cmd, "advancestat") == 0) {
		char *stat_str;
		char *xp_str;
		int stat = -1;
		int xp = 0;

		stat_str = strtok(NULL, " ");
		xp_str = strtok(NULL, " ");

		if (stat_str == NULL || xp_str == NULL) {
			player_send_message(p, "Usage: advancestat attack 4000");
			return;
		}

		for (int i = 0; i < MAX_SKILL_ID; ++i) {
			if (strcasecmp(stat_str, skill_names[i]) == 0) {
				stat = i;
				break;
			}
		}

		if (stat == -1) {
			char mes[32];

			(void)snprintf(mes, sizeof(mes),
			    "Unknown stat name: %s", stat_str);
			player_send_message(p, mes);
			return;
		}

		xp = strtol(xp_str, NULL, 10);

		stat_advance(p, stat, xp, 0);
	}
}
