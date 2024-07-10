#ifndef DATABASE_H
#define DATABASE_H

#include "../entity.h"

/* TODO ifdef WITH_SQLITE */
#include <sqlite3.h>

struct database {
	const char *filename;
	bool use_wal;

	sqlite3 *db;

	sqlite3_stmt *check_username;

	sqlite3_stmt *get_player;
	sqlite3_stmt *get_player_variables;
	sqlite3_stmt *get_player_contacts;
	sqlite3_stmt *get_player_inventory;
	sqlite3_stmt *get_player_bank;

	sqlite3_stmt *new_player;
	sqlite3_stmt *save_player;
	sqlite3_stmt *save_player_variable;
	sqlite3_stmt *save_player_contact;
	sqlite3_stmt *save_player_inventory;
	sqlite3_stmt *save_player_bank;

	sqlite3_stmt *delete_player_variables;
	sqlite3_stmt *delete_player_contacts;
	sqlite3_stmt *delete_player_inventory;
	sqlite3_stmt *delete_player_bank;
};
/* endif */

int database_init(struct database *);
/* return -1 on error, 0 on existent username, 1 if successful */
int database_new_player(struct database *, struct player *);
/* return -1 on error, 0 on non-existent username, 1 if successful */
int database_load_player(struct database *, struct player *);
int database_save_player(struct database *, struct player *);

#endif
