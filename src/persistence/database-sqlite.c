#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../server.h"
#include "../stat.h"
#include "../utility.h"
#include "database.h"

enum transaction_stage {
	TRANSACTION_START,
	TRANSACTION_END
};

static int db_prepare_query(sqlite3 *, sqlite3_stmt **, const char *);
static int db_transaction(sqlite3 *, enum transaction_stage);

static int stmt_bind_text(sqlite3 *, sqlite3_stmt *, int, const char *);
static int stmt_bind_int(sqlite3 *, sqlite3_stmt *, int, int);
static int stmt_bind_uint64(sqlite3 *, sqlite3_stmt *, int, uint64_t);
static int stmt_reset(sqlite3 *, sqlite3_stmt *);

static int database_load_player_variables(struct database *, struct player *,
	int);
static int database_load_player_contacts(struct database *, struct player *,
	int);
static int database_load_player_inventory(struct database *, struct player *,
	int);
static int database_load_player_bank(struct database *, struct player *, int);

static int database_delete_player_table(struct database *, sqlite3_stmt *, int);

static int database_save_player_variables(struct database *, struct player *,
	int);
static int database_save_player_contacts(struct database *, struct player *,
	int, bool);
static int database_save_player_inventory(struct database *, struct player *,
	int);
static int database_save_player_bank(struct database *, struct player *, int);

static int
db_prepare_query(sqlite3 *db, sqlite3_stmt **stmt, const char *query)
{
	if (sqlite3_prepare_v2(db, query, -1, stmt, NULL) != SQLITE_OK) {
		fprintf(stderr, "unable to prepare statement: %s\n",
			sqlite3_errmsg(db));

		return -1;
	}

	return 0;
}

static int
db_transaction(sqlite3 *db, enum transaction_stage stage)
{
	const char *query =
		stage == TRANSACTION_START ? "BEGIN TRANSACTION" : "COMMIT";

	if (sqlite3_exec(db, query, NULL, NULL, NULL) != SQLITE_OK) {
		fprintf(stderr, "unable to perform transaction: %s\n",
			sqlite3_errmsg(db));

		return -1;
	}

	return 0;
}

static int
stmt_bind_text(sqlite3 *db, sqlite3_stmt *stmt, int index, const char *text)
{
	if (sqlite3_bind_text(stmt, index, text, -1, SQLITE_STATIC) != SQLITE_OK) {
		fprintf(stderr, "unable to bind text: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	return 0;
}

static int
stmt_bind_int(sqlite3 *db, sqlite3_stmt *stmt, int index, int value)
{
	if (sqlite3_bind_int(stmt, index, value) != SQLITE_OK) {
		fprintf(stderr, "unable to bind int: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	return 0;
}

static int
stmt_bind_uint64(sqlite3 *db, sqlite3_stmt *stmt, int index, uint64_t value)
{
	if (sqlite3_bind_int64(stmt, index, value) != SQLITE_OK) {
		fprintf(stderr, "unable to bind int64: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	return 0;
}

/* reset and clear bindings for next use */
static int
stmt_reset(sqlite3 *db, sqlite3_stmt *stmt)
{
	if (sqlite3_clear_bindings(stmt) != SQLITE_OK) {
		fprintf(stderr, "unable to clear statement bindings: %s\n",
			sqlite3_errmsg(db));

		return -1;
	}

	if (sqlite3_reset(stmt) != SQLITE_OK) {
		fprintf(stderr, "unable to reset statement: %s\n", sqlite3_errmsg(db));
		return -1;
	}

	return 0;
}

static int
database_load_player_variables(struct database *database, struct player *player,
	int player_id)
{
	if (stmt_bind_int(database->db, database->get_player_variables, 1,
			player_id) == -1) {
		return -1;
	}

	int res;

	while ((res = sqlite3_step(database->get_player_variables)) == SQLITE_ROW) {
		int column_index = 0;

		const unsigned char *key =
			sqlite3_column_text(database->get_player_variables, column_index++);

		int value =
			sqlite3_column_int(database->get_player_variables, column_index++);

		player_variable_set(player, (char*)key, (int32_t)value);
	}

	if (res != SQLITE_DONE) {
		fprintf(stderr, "error fetching variables: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, database->get_player_variables) == -1) {
		return -1;
	}

	return 0;
}

static int
database_load_player_contacts(struct database *database, struct player *player,
	int player_id)
{
	if (stmt_bind_int(database->db, database->get_player_contacts, 1,
			player_id) == -1) {
		return -1;
	}

	int res;

	while ((res = sqlite3_step(database->get_player_contacts)) == SQLITE_ROW) {
		int column_index = 0;

		int is_ignore = sqlite3_column_int(database->get_player_contacts,
			column_index++);

		int slot = sqlite3_column_int(database->get_player_contacts,
			column_index++);

		const unsigned char *contact_name =
			sqlite3_column_text(database->get_player_contacts, column_index++);

		int64_t contact = mod37_nameenc((char*)contact_name);

		if (is_ignore) {
			player->ignore_list[slot] = contact;
			player->ignore_count++;
		} else {
			player->friend_list[slot] = contact;
			player->friend_count++;
		}
	}

	if (res != SQLITE_DONE) {
		fprintf(stderr, "error fetching contacts: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, database->get_player_contacts) == -1) {
		return -1;
	}

	return 0;
}

static int
database_load_player_inventory(struct database *database, struct player *player,
	int player_id)
{
	if (stmt_bind_int(database->db, database->get_player_inventory, 1,
			player_id) == -1) {
		return -1;
	}

	int res;

	while ((res = sqlite3_step(database->get_player_inventory)) == SQLITE_ROW) {
		int column_index = 0;

		int slot = sqlite3_column_int(database->get_player_inventory,
			column_index++);

		int item_id = sqlite3_column_int(database->get_player_inventory,
			column_index++);

		int amount = sqlite3_column_int(database->get_player_inventory,
			column_index++);

		int is_worn = sqlite3_column_int(database->get_player_inventory,
			column_index++);

		player->inventory[slot].id = item_id;
		player->inventory[slot].stack = amount;
		player->inventory[slot].worn = is_worn;

		player->inv_count++;
	}

	if (res != SQLITE_DONE) {
		fprintf(stderr, "error fetching inventory: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, database->get_player_inventory) == -1) {
		return -1;
	}

	return 0;
}

static int
database_load_player_bank(struct database *database, struct player *player,
	int player_id)
{
	if (stmt_bind_int(database->db, database->get_player_bank, 1,
			player_id) == -1) {
		return -1;
	}

	int res;

	while ((res = sqlite3_step(database->get_player_bank)) == SQLITE_ROW) {
		int column_index = 0;

		int slot = sqlite3_column_int(database->get_player_bank,
			column_index++);

		int item_id = sqlite3_column_int(database->get_player_bank,
			column_index++);

		int amount = sqlite3_column_int(database->get_player_bank,
			column_index++);

		player->bank[slot].id = item_id;
		player->bank[slot].amount = amount;

		player->bank_count++;
	}

	if (res != SQLITE_DONE) {
		fprintf(stderr, "error fetching bank: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, database->get_player_bank) == -1) {
		return -1;
	}

	return 0;
}

static int
database_delete_player_table(struct database * database, sqlite3_stmt *stmt,
	int player_id)
{
	if (stmt_bind_int(database->db, stmt, 1, player_id) == -1) {
		return -1;
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		fprintf(stderr, "error deleting player table: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, stmt) == -1) {
		return -1;
	}

	return 0;
}

static int
database_save_player_contacts(struct database *database, struct player *player,
	int player_id, bool is_ignore)
{
	int contact_count = is_ignore ? player->ignore_count : player->friend_count;
	int64_t *contacts = is_ignore ? player->ignore_list : player->friend_list;

	for (int i = 0; i < contact_count; i++) {
		int bind_index = 1;

		if (stmt_bind_int(database->db, database->save_player_contact,
				bind_index++, player_id) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_contact,
				bind_index++, is_ignore) == -1) {
			return -1;
		}

		/* slot */
		if (stmt_bind_int(database->db, database->save_player_contact,
				bind_index++, i) == -1) {
			return -1;
		}

		char contact_name[32];
		mod37_namedec(contacts[i], contact_name);

		if (stmt_bind_text(database->db, database->save_player_contact,
				bind_index++, contact_name) == -1) {
			return -1;
		}

		if (sqlite3_step(database->save_player_contact) != SQLITE_DONE) {
			fprintf(stderr, "unable to save player contact: %s\n",
				sqlite3_errmsg(database->db));

			return -1;
		}

		if (stmt_reset(database->db, database->save_player_contact) == -1) {
			return -1;
		}
	}

	return 0;
}

static int
database_save_player_variables(struct database *database, struct player *player,
	int player_id)
{
	if (database_delete_player_table(database,
			database->delete_player_variables, player_id) == -1) {
		return -1;
	}

	for (size_t i = 0; i < player->variable_count; i++) {
		int bind_index = 1;

		if (stmt_bind_int(database->db, database->save_player_variable,
				bind_index++, player_id) == -1) {
			return -1;
		}

		if (stmt_bind_text(database->db, database->save_player_variable,
				bind_index++, player->variables[i].name) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_variable,
				bind_index++, player->variables[i].value) == -1) {
			return -1;
		}

		if (sqlite3_step(database->save_player_variable) != SQLITE_DONE) {
			fprintf(stderr, "unable to save player variable: %s\n",
				sqlite3_errmsg(database->db));

			return -1;
		}

		if (stmt_reset(database->db, database->save_player_variable) == -1) {
			return -1;
		}
	}

	return 0;
}

static int
database_save_player_inventory(struct database *database, struct player *player,
	int player_id)
{
	if (database_delete_player_table(database,
			database->delete_player_inventory, player_id) == -1) {
		return -1;
	}

	for (int i = 0; i < player->inv_count; i++) {
		int bind_index = 1;

		if (stmt_bind_int(database->db, database->save_player_inventory,
				bind_index++, player_id) == -1) {
			return -1;
		}

		/* slot */
		if (stmt_bind_int(database->db, database->save_player_inventory,
				bind_index++, i) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_inventory,
				bind_index++, player->inventory[i].id) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_inventory,
				bind_index++, player->inventory[i].stack) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_inventory,
				bind_index++, player->inventory[i].worn) == -1) {
			return -1;
		}

		if (sqlite3_step(database->save_player_inventory) != SQLITE_DONE) {
			fprintf(stderr, "unable to save player inventory item: %s\n",
				sqlite3_errmsg(database->db));

			return -1;
		}

		if (stmt_reset(database->db, database->save_player_inventory) == -1) {
			return -1;
		}
	}

	return 0;
}

static int
database_save_player_bank(struct database *database, struct player *player,
	int player_id)
{
	if (database_delete_player_table(database, database->delete_player_bank,
			player_id) == -1) {
		return -1;
	}

	for (int i = 0; i < player->bank_count; i++) {
		int bind_index = 1;

		if (stmt_bind_int(database->db, database->save_player_bank,
				bind_index++, player_id) == -1) {
			return -1;
		}

		/* slot */
		if (stmt_bind_int(database->db, database->save_player_bank,
				bind_index++, i) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_bank,
				bind_index++, player->bank[i].id) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player_bank,
				bind_index++, player->bank[i].amount) == -1) {
			return -1;
		}

		if (sqlite3_step(database->save_player_bank) != SQLITE_DONE) {
			fprintf(stderr, "unable to save player bank item: %s\n",
				sqlite3_errmsg(database->db));

			return -1;
		}

		if (stmt_reset(database->db, database->save_player_bank) == -1) {
			return -1;
		}
	}

	return 0;
}

int
database_init(struct database *database)
{
	if (sqlite3_open(database->filename, &database->db) != SQLITE_OK) {
		fprintf(stderr, "cannot open database: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	/* https://www.sqlite.org/wal.html */
	const char *wal_query = database->use_wal ?
		"PRAGMA journal_mode=WAL;" : "PRAGMA journal_mode=delete;";

	if (sqlite3_exec(database->db, wal_query, NULL, NULL, NULL) != SQLITE_OK) {
		fprintf(stderr, "unable to %s WAL: %s\n",
			database->use_wal ? "enable" : "disable",
			sqlite3_errmsg(database->db));

		return -1;
	}

	char *create_sql = buffer_file("./schema.sql");

	if (create_sql == NULL) {
		return -1;
	}

	if (sqlite3_exec(database->db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
		fprintf(stderr, "unable to create tables: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	free(create_sql);

	/* selects */
	if (db_prepare_query(database->db, &database->check_username,
			"SELECT 1 FROM `players` WHERE `username` = ?") == -1) {
		return -1;
	}

	char get_query[1024] = "SELECT `id`, `password`, `rpg_class`, "
	    "`login_date`, `x`, `y`, `quest_points`, `camera_auto`, "
	    "`one_mouse_button`, `block_public`, `block_private`, "
	    "`block_trade`, `block_duel`, `hair_colour`, `top_colour`, "
	    "`leg_colour`, `skin_colour`, `head_sprite`, `body_sprite`, "
	    "`skull_timer`, ";

	for (int i = 0; i < MAX_SKILL_ID; i++) {
		int remaining = sizeof(get_query) - strlen(get_query);

		snprintf(get_query + strlen(get_query), remaining,
			"`%s_current`, `%s_xp`%s ", skill_names[i], skill_names[i],
			(i < MAX_SKILL_ID - 1) ? "," : "");
	}

	snprintf(get_query + strlen(get_query),
		sizeof(get_query) - strlen(get_query),
		"FROM `players` WHERE `username` = ?");

	if (db_prepare_query(database->db, &database->get_player,
			get_query) == -1){
		return -1;
	}

	const char *get_variables_query = "SELECT `key`, `value` "
		"FROM `players_variables` WHERE `player_id` = ?";

	if (db_prepare_query(database->db, &database->get_player_variables,
			get_variables_query) == -1) {
		return -1;
	}

	const char *get_contacts_query = "SELECT `is_ignore`, `slot`, `contact` "
		"FROM `players_contacts` WHERE `player_id` = ?";

	if (db_prepare_query(database->db, &database->get_player_contacts,
			get_contacts_query) == -1) {
		return -1;
	}

	const char *get_inventory_query = "SELECT `slot`, `item_id`, `amount`, "
		"`is_worn` FROM `players_inventory` WHERE `player_id` = ?";

	if (db_prepare_query(database->db, &database->get_player_inventory,
			get_inventory_query) == -1) {
		return -1;
	}

	const char *get_bank_query = "SELECT `slot`, `item_id`, `amount` "
		"FROM `players_bank` WHERE `player_id` = ?";

	if (db_prepare_query(database->db, &database->get_player_bank,
			get_bank_query) == -1) {
		return -1;
	}

	/* update/inserts */
	char *new_query = "INSERT INTO `players` (`username`, `password`) "
		"VALUES(?, ?)";

	if (db_prepare_query(database->db, &database->new_player,
			new_query) == -1) {
		return -1;
	}

	char save_query[2048] = "UPDATE `players` SET `rpg_class` = ?, "
		"`login_date` = ?, `x` = ?, `y` = ?, `quest_points` = ?, "
		"`camera_auto` = ?, `one_mouse_button` = ?, `block_public` = ?, "
		"`block_private` = ?, `block_trade` = ?, `block_duel` = ?, "
		"`hair_colour` = ?, `top_colour` = ?, `leg_colour` = ?, "
		"`skin_colour` = ?, `head_sprite` = ?, `body_sprite` = ?, "
		"`skull_timer` = ?, ";

	for (int i = 0; i < MAX_SKILL_ID; i++) {
		int remaining = sizeof(save_query) - strlen(save_query);

		snprintf(save_query + strlen(save_query), remaining,
			"`%s_current` = ?, `%s_xp` = ?, ",
			skill_names[i], skill_names[i]);
	}

	snprintf(save_query + strlen(save_query),
		sizeof(save_query) - strlen(save_query),
		"`total_level` = ? WHERE `username` = ? RETURNING `id`");

	if (db_prepare_query(database->db, &database->save_player,
			save_query) == -1) {
		return -1;
	}

	const char *variable_save_query = "INSERT INTO `players_variables` "
		"(`player_id`, `key`, `value`) VALUES(?, ?, ?)";

	if (db_prepare_query(database->db, &database->save_player_variable,
			variable_save_query) == -1) {
		return -1;
	}

	const char *contact_save_query = "INSERT INTO `players_contacts` "
		"(`player_id`, `is_ignore`, `slot`, `contact`) VALUES(?, ?, ?, ?)";

	if (db_prepare_query(database->db, &database->save_player_contact,
			contact_save_query) == -1) {
		return -1;
	}

	const char *inventory_save_query = "INSERT INTO `players_inventory` "
		"(`player_id`, `slot`, `item_id`, `amount`, `is_worn`) "
		"VALUES(?, ?, ?, ?, ?)";

	if (db_prepare_query(database->db, &database->save_player_inventory,
			inventory_save_query) == -1) {
		return -1;
	}

	const char *bank_save_query = "INSERT INTO `players_bank` "
		"(`player_id`, `slot`, `item_id`, `amount`) VALUES(?, ?, ?, ?)";

	if (db_prepare_query(database->db, &database->save_player_bank,
			bank_save_query) == -1) {
		return -1;
	}

	/* deletes */
	if (db_prepare_query(database->db, &database->delete_player_variables,
			"DELETE FROM `players_variables` WHERE `player_id` = ?") == -1) {
		return -1;
	}

	if (db_prepare_query(database->db, &database->delete_player_contacts,
			"DELETE FROM `players_contacts` WHERE `player_id` = ?") == -1) {
		return -1;
	}

	if (db_prepare_query(database->db, &database->delete_player_inventory,
			"DELETE FROM `players_inventory` WHERE `player_id` = ?") == -1) {
		return -1;
	}

	if (db_prepare_query(database->db, &database->delete_player_bank,
			"DELETE FROM `players_bank` WHERE `player_id` = ?") == -1) {
		return -1;
	}

	return 0;
}

int
database_new_player(struct database *database, struct player *player)
{
	uint8_t salt[32];
	char username[32];
	char encoded_pwd[1024];

	if (db_transaction(database->db, TRANSACTION_START) == -1) {
		return -1;
	}

	mod37_namedec(player->name, username);

	if (stmt_bind_text(database->db, database->check_username, 1,
			username) == -1) {
		return -1;
	}

	int res;

	res = sqlite3_step(database->check_username);

	printf("trying to create %s\n", username);

	/* user found */
	if (res == SQLITE_ROW) {
		if (stmt_reset(database->db, database->check_username) == -1) {
			return -1;
		}

		if (db_transaction(database->db, TRANSACTION_END) == -1) {
			return -1;
		}

		return 0;
	}

	if (res != SQLITE_DONE) {
		fprintf(stderr, "error checking username in database: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, database->check_username) == -1) {
		return -1;
	}

	int bind_index = 1;

	if (stmt_bind_text(database->db, database->new_player, bind_index++,
			username) == -1) {
		return -1;
	}

	/* start to prepare a password hash */

	gen_salt(salt, sizeof(salt));

	if (pwhash_hash(&player->mob.server->hash, (char *)salt,
	    player->password, encoded_pwd, sizeof(encoded_pwd)) == -1) {
		printf("password hashing failure\n");
		return -1;
	}

	printf("salt is %s\n", salt);
	printf("hash is %s\n", encoded_pwd);

	if (stmt_bind_text(database->db, database->new_player, bind_index++,
			encoded_pwd) == -1) {
		return -1;
	}

	if (sqlite3_step(database->new_player) != SQLITE_DONE) {
		fprintf(stderr, "unable to create player: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	if (stmt_reset(database->db, database->new_player) == -1) {
		return -1;
	}

	if (db_transaction(database->db, TRANSACTION_END) == -1) {
		return -1;
	}

	return 1;
}

int
database_load_player(struct database *database, struct player *player)
{
	char username[32];
	const char *pwhash;
	int player_id;
	int res, ret;

	if (db_transaction(database->db, TRANSACTION_START) == -1) {
		return -1;
	}

	mod37_namedec(player->name, username);

	if (stmt_bind_text(database->db, database->get_player, 1, username) == -1) {
		return -1;
	}

	res = sqlite3_step(database->get_player);

	/* successful, but no user found */
	if (res == SQLITE_DONE) {
		if (stmt_reset(database->db, database->get_player) == -1) {
			return -1;
		}

		if (db_transaction(database->db, TRANSACTION_END) == -1) {
			return -1;
		}

		return 0;
	}

	/* no user and an error occurred */
	if (res != SQLITE_ROW) {
		fprintf(stderr, "error loading player from database: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	int column_index = 0;

	player_id = sqlite3_column_int(database->get_player, column_index++);

	pwhash = (const char *)sqlite3_column_text(database->get_player, column_index++);

	if (pwhash_verify(&player->mob.server->hash,
	    pwhash, player->password) == -1) {
		printf("invalid password\n");
		ret = 0;
		goto end;
	}

	player->rpg_class =
		sqlite3_column_int(database->get_player, column_index++);
	player->login_date =
		(uint64_t)sqlite3_column_int64(database->get_player, column_index++);
	player->mob.x = sqlite3_column_int(database->get_player, column_index++);
	player->mob.y = sqlite3_column_int(database->get_player, column_index++);
	player->quest_points = sqlite3_column_int(database->get_player,
		column_index++);
	/* camera auto */
	player->client_settings[0] =
		sqlite3_column_int(database->get_player, column_index++);
	/* one mouse button */
	player->client_settings[1] =
		sqlite3_column_int(database->get_player, column_index++);
	player->block_public =
		sqlite3_column_int(database->get_player, column_index++);
	player->block_private =
		sqlite3_column_int(database->get_player, column_index++);
	player->block_trade =
		sqlite3_column_int(database->get_player, column_index++);
	player->block_duel =
		sqlite3_column_int(database->get_player, column_index++);
	player->hair_colour =
		sqlite3_column_int(database->get_player, column_index++);
	player->top_colour =
		sqlite3_column_int(database->get_player, column_index++);
	player->leg_colour =
		sqlite3_column_int(database->get_player, column_index++);
	player->skin_colour =
		sqlite3_column_int(database->get_player, column_index++);
	player->sprites_base[ANIM_SLOT_HEAD] =
		sqlite3_column_int(database->get_player, column_index++);
	player->sprites_base[ANIM_SLOT_BODY] =
		sqlite3_column_int(database->get_player, column_index++);
	player->skull_timer =
		sqlite3_column_int(database->get_player, column_index++);

	for (int i = 0; i < MAX_SKILL_ID; i++) {
		player->mob.cur_stats[i] =
			sqlite3_column_int(database->get_player, column_index++);

		player->experience[i] =
			sqlite3_column_int(database->get_player, column_index++);

		player->mob.base_stats[i] = xp_to_level(player->experience[i]);
	}

	if (player->skull_timer > 0) {
		player->skulled = true;
	}

	if (stmt_reset(database->db, database->get_player) == -1) {
		ret = -1;
		goto end;
	}

	if (database_load_player_variables(database, player, player_id) == -1) {
		ret = -1;
		goto end;
	}

	if (database_load_player_contacts(database, player, player_id) == -1) {
		ret = -1;
		goto end;
	}

	if (database_load_player_inventory(database, player, player_id) == -1) {
		ret = -1;
		goto end;
	}

	if (database_load_player_bank(database, player, player_id) == -1) {
		ret = -1;
		goto end;
	}

	if (db_transaction(database->db, TRANSACTION_END) == -1) {
		ret = -1;
		goto end;
	}

	return 1;

end:
	(void)stmt_reset(database->db, database->get_player);
	(void)db_transaction(database->db, TRANSACTION_END);
	return ret;
}

int
database_save_player(struct database *database, struct player *player)
{
	if (db_transaction(database->db, TRANSACTION_START) == -1) {
		return -1;
	}

	int bind_index = 1;

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->rpg_class) == -1) {
		return -1;
	}

	if (stmt_bind_uint64(database->db, database->save_player, bind_index++,
			player->login_date) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->mob.x) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->mob.y) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->quest_points) == -1) {
		return -1;
	}

	/* camera auto */
	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->client_settings[0]) == -1) {
		return -1;
	}

	/* one mouse button */
	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->client_settings[2]) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->block_public) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->block_private) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->block_trade) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->block_duel) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->hair_colour) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->top_colour) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->leg_colour) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->skin_colour) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->sprites_base[ANIM_SLOT_HEAD]) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->sprites_base[ANIM_SLOT_BODY]) == -1) {
		return -1;
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			player->skull_timer) == -1) {
		return -1;
	}

	int total_level = 0;

	for (int i = 0; i < MAX_SKILL_ID; i++) {
		if (stmt_bind_int(database->db, database->save_player, bind_index++,
				player->mob.cur_stats[i]) == -1) {
			return -1;
		}

		if (stmt_bind_int(database->db, database->save_player, bind_index++,
				player->experience[i]) == -1) {
			return -1;
		}

		total_level += player->mob.base_stats[i];
	}

	if (stmt_bind_int(database->db, database->save_player, bind_index++,
			total_level) == -1) {
		return -1;
	}

	char username[32];
	mod37_namedec(player->name, username);

	if (stmt_bind_text(database->db, database->save_player, bind_index++,
			username) == -1) {
		return -1;
	}

	if (sqlite3_step(database->save_player) != SQLITE_ROW) {
		fprintf(stderr, "error saving player: %s\n",
			sqlite3_errmsg(database->db));

		return -1;
	}

	int player_id = sqlite3_column_int(database->save_player, 0);

	if (stmt_reset(database->db, database->save_player) == -1) {
		return -1;
	}

	if (database_save_player_variables(database, player, player_id) == -1) {
		return -1;
	}

	if (database_delete_player_table(database, database->delete_player_contacts,
			player_id) == -1) {
		return -1;
	}

	if (database_save_player_contacts(database, player, player_id,
			false) == -1) {
		return -1;
	}

	if (database_save_player_contacts(database, player, player_id,
			true) == -1) {
		return -1;
	}

	if (database_save_player_inventory(database, player, player_id) == -1) {
		return -1;
	}

	if (database_save_player_bank(database, player, player_id) == -1) {
		return -1;
	}

	if (db_transaction(database->db, TRANSACTION_END) == -1) {
		return -1;
	}

	return 0;
}
