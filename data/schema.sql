BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "players_variables" (
	"player_id"	INTEGER NOT NULL,
	"key"	TEXT NOT NULL,
	"value"	INTEGER NOT NULL DEFAULT 0,
	FOREIGN KEY("player_id") REFERENCES "players"("id"),
	PRIMARY KEY("player_id","key")
);
CREATE TABLE IF NOT EXISTS "players_contacts" (
	"player_id"	INTEGER NOT NULL,
	"is_ignore"	INTEGER NOT NULL DEFAULT 0,
	"slot"	INTEGER NOT NULL,
	"contact"	TEXT NOT NULL,
	FOREIGN KEY("player_id") REFERENCES "players"("id"),
	UNIQUE("player_id","contact"),
	PRIMARY KEY("player_id","is_ignore","slot")
);
CREATE TABLE IF NOT EXISTS "players_bank" (
	"player_id"	INTEGER NOT NULL,
	"slot"	INTEGER NOT NULL,
	"item_id"	INTEGER NOT NULL,
	"amount"	INTEGER NOT NULL,
	PRIMARY KEY("player_id","item_id"),
	UNIQUE("player_id","slot"),
	FOREIGN KEY("player_id") REFERENCES "players"("id")
);
CREATE TABLE IF NOT EXISTS "players" (
	"id"	INTEGER NOT NULL UNIQUE,
	"username"	TEXT NOT NULL UNIQUE,
	"password"	TEXT NOT NULL,
	"rpg_class"	INTEGER NOT NULL DEFAULT 0,
	"rank"	INTEGER NOT NULL DEFAULT 0,
	"creation_ip"	TEXT NOT NULL DEFAULT '127.0.0.1',
	"login_ip"	TEXT NOT NULL DEFAULT '',
	"creation_date"	INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),
	"login_date"	INTEGER NOT NULL DEFAULT 0,
	"ban_end_date"	INTEGER NOT NULL DEFAULT 0,
	"mute_end_date"	INTEGER NOT NULL DEFAULT 0,
	"x"	INTEGER NOT NULL DEFAULT 120,
	"y"	INTEGER NOT NULL DEFAULT 648,
	"quest_points"	INTEGER NOT NULL DEFAULT 0,
	"camera_auto"	INTEGER NOT NULL DEFAULT 0,
	"one_mouse_button"	INTEGER NOT NULL DEFAULT 1,
	"block_public"	INTEGER NOT NULL DEFAULT 0,
	"block_private"	INTEGER NOT NULL DEFAULT 0,
	"block_trade"	INTEGER NOT NULL DEFAULT 0,
	"block_duel"	INTEGER NOT NULL DEFAULT 0,
	"hair_colour"	INTEGER NOT NULL DEFAULT 2,
	"top_colour"	INTEGER NOT NULL DEFAULT 8,
	"leg_colour"	INTEGER NOT NULL DEFAULT 14,
	"skin_colour"	INTEGER NOT NULL DEFAULT 0,
	"head_sprite"	INTEGER NOT NULL DEFAULT 1,
	"body_sprite"	INTEGER NOT NULL DEFAULT 2,
	"skull_timer"	INTEGER NOT NULL DEFAULT 0,
	"attack_current"	INTEGER NOT NULL DEFAULT 1,
	"defence_current"	INTEGER NOT NULL DEFAULT 1,
	"strength_current"	INTEGER NOT NULL DEFAULT 1,
	"hitpoints_current"	INTEGER NOT NULL DEFAULT 10,
	"ranged_current"	INTEGER NOT NULL DEFAULT 1,
	"prayer_current"	INTEGER NOT NULL DEFAULT 1,
	"magic_current"	INTEGER NOT NULL DEFAULT 1,
	"cooking_current"	INTEGER NOT NULL DEFAULT 1,
	"woodcutting_current"	INTEGER NOT NULL DEFAULT 1,
	"fletching_current"	INTEGER NOT NULL DEFAULT 1,
	"fishing_current"	INTEGER NOT NULL DEFAULT 1,
	"firemaking_current"	INTEGER NOT NULL DEFAULT 1,
	"crafting_current"	INTEGER NOT NULL DEFAULT 1,
	"smithing_current"	INTEGER NOT NULL DEFAULT 1,
	"mining_current"	INTEGER NOT NULL DEFAULT 1,
	"herblaw_current"	INTEGER NOT NULL DEFAULT 1,
	"attack_xp"	INTEGER NOT NULL DEFAULT 0,
	"defence_xp"	INTEGER NOT NULL DEFAULT 0,
	"strength_xp"	INTEGER NOT NULL DEFAULT 0,
	"hitpoints_xp"	INTEGER NOT NULL DEFAULT 4616,
	"ranged_xp"	INTEGER NOT NULL DEFAULT 0,
	"prayer_xp"	INTEGER NOT NULL DEFAULT 0,
	"magic_xp"	INTEGER NOT NULL DEFAULT 0,
	"cooking_xp"	INTEGER NOT NULL DEFAULT 0,
	"woodcutting_xp"	INTEGER NOT NULL DEFAULT 0,
	"fletching_xp"	INTEGER NOT NULL DEFAULT 0,
	"fishing_xp"	INTEGER NOT NULL DEFAULT 0,
	"firemaking_xp"	INTEGER NOT NULL DEFAULT 0,
	"crafting_xp"	INTEGER NOT NULL DEFAULT 0,
	"smithing_xp"	INTEGER NOT NULL DEFAULT 0,
	"mining_xp"	INTEGER NOT NULL DEFAULT 0,
	"herblaw_xp"	INTEGER NOT NULL DEFAULT 0,
	"total_level"	INTEGER NOT NULL DEFAULT 18,
	"attack_rank"	INTEGER NOT NULL DEFAULT 0,
	"defence_rank"	INTEGER NOT NULL DEFAULT 0,
	"strength_rank"	INTEGER NOT NULL DEFAULT 0,
	"hitpoints_rank"	INTEGER NOT NULL DEFAULT 0,
	"ranged_rank"	INTEGER NOT NULL DEFAULT 0,
	"prayer_rank"	INTEGER NOT NULL DEFAULT 0,
	"magic_rank"	INTEGER NOT NULL DEFAULT 0,
	"cooking_rank"	INTEGER NOT NULL DEFAULT 0,
	"woodcutting_rank"	INTEGER NOT NULL DEFAULT 0,
	"fletching_rank"	INTEGER NOT NULL DEFAULT 0,
	"fishing_rank"	INTEGER NOT NULL DEFAULT 0,
	"firemaking_rank"	INTEGER NOT NULL DEFAULT 0,
	"crafting_rank"	INTEGER NOT NULL DEFAULT 0,
	"smithing_rank"	INTEGER NOT NULL DEFAULT 0,
	"mining_rank"	INTEGER NOT NULL DEFAULT 0,
	"herblaw_rank"	INTEGER NOT NULL DEFAULT 0,
	"total_rank"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "players_inventory" (
	"player_id"	INTEGER NOT NULL,
	"slot"	INTEGER NOT NULL,
	"item_id"	INTEGER NOT NULL,
	"amount"	INTEGER NOT NULL,
	"is_worn"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("player_id","slot"),
	FOREIGN KEY("player_id") REFERENCES "players"("id")
);
CREATE INDEX IF NOT EXISTS "player_id_bank" ON "players_bank" (
	"player_id"
);
CREATE INDEX IF NOT EXISTS "player_id_variables" ON "players_variables" (
	"player_id"
);
CREATE INDEX IF NOT EXISTS "player_id_contacts" ON "players_contacts" (
	"player_id"
);
CREATE INDEX IF NOT EXISTS "player_username" ON "players" (
	"username"
);
CREATE INDEX IF NOT EXISTS "player_id_inventory" ON "players_inventory" (
	"player_id"
);
COMMIT;
BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "players" (
	"id"	INTEGER NOT NULL UNIQUE,
	"username"	TEXT NOT NULL UNIQUE,
	"password"	TEXT NOT NULL,
	"rank"	INTEGER NOT NULL DEFAULT 0,
	"creation_ip"	TEXT NOT NULL DEFAULT '127.0.0.1',
	"creation_date"	INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),
	"login_ip"	TEXT NOT NULL,
	"ban_end_date"	INTEGER NOT NULL DEFAULT 0,
	"mute_end_date"	INTEGER NOT NULL DEFAULT 0,
	"x"	INTEGER NOT NULL DEFAULT 122,
	"y"	INTEGER NOT NULL DEFAULT 657,
	"quest_points"	INTEGER NOT NULL DEFAULT 0,
	"camera_auto"	INTEGER NOT NULL DEFAULT 0,
	"one_mouse_button"	INTEGER NOT NULL DEFAULT 0,
	"block_chat"	INTEGER NOT NULL DEFAULT 0,
	"block_private_chat"	INTEGER NOT NULL DEFAULT 0,
	"block_trade"	INTEGER NOT NULL DEFAULT 0,
	"hair_colour"	INTEGER NOT NULL DEFAULT 2,
	"top_colour"	INTEGER NOT NULL DEFAULT 8,
	"trouser_colour"	INTEGER NOT NULL DEFAULT 14,
	"skin_colour"	INTEGER NOT NULL DEFAULT 0,
	"head_sprite"	INTEGER NOT NULL DEFAULT 1,
	"body_sprite"	INTEGER NOT NULL DEFAULT 2,
	"skulled"	INTEGER NOT NULL DEFAULT 0,
	"attack_current"	INTEGER NOT NULL DEFAULT 1,
	"defense_current"	INTEGER NOT NULL DEFAULT 1,
	"strength_current"	INTEGER NOT NULL DEFAULT 1,
	"hits_current"	INTEGER NOT NULL DEFAULT 10,
	"ranged_current"	INTEGER NOT NULL DEFAULT 1,
	"prayer_current"	INTEGER NOT NULL DEFAULT 1,
	"magic_current"	INTEGER NOT NULL DEFAULT 1,
	"cooking_current"	INTEGER NOT NULL DEFAULT 1,
	"woodcutting_current"	INTEGER NOT NULL DEFAULT 1,
	"fishing_current"	INTEGER NOT NULL DEFAULT 1,
	"crafting_current"	INTEGER NOT NULL DEFAULT 1,
	"smithing_current"	INTEGER NOT NULL DEFAULT 1,
	"mining_current"	INTEGER NOT NULL DEFAULT 1,
	"attack_xp"	INTEGER NOT NULL DEFAULT 0,
	"defense_xp"	INTEGER NOT NULL DEFAULT 0,
	"strength_xp"	INTEGER NOT NULL DEFAULT 0,
	"hits_xp"	INTEGER NOT NULL DEFAULT 4616,
	"ranged_xp"	INTEGER NOT NULL DEFAULT 0,
	"prayer_xp"	INTEGER NOT NULL DEFAULT 0,
	"magic_xp"	INTEGER NOT NULL DEFAULT 0,
	"cooking_xp"	INTEGER NOT NULL DEFAULT 0,
	"woodcutting_xp"	INTEGER NOT NULL DEFAULT 0,
	"fishing_xp"	INTEGER NOT NULL DEFAULT 0,
	"crafting_xp"	INTEGER NOT NULL DEFAULT 0,
	"smithing_xp"	INTEGER NOT NULL DEFAULT 0,
	"mining_xp"	INTEGER NOT NULL DEFAULT 0,
	"total_level"	INTEGER NOT NULL DEFAULT 18,
	"attack_rank"	INTEGER NOT NULL DEFAULT 0,
	"defense_rank"	INTEGER NOT NULL DEFAULT 0,
	"strength_rank"	INTEGER NOT NULL DEFAULT 0,
	"hits_rank"	INTEGER NOT NULL DEFAULT 0,
	"ranged_rank"	INTEGER NOT NULL DEFAULT 0,
	"prayer_rank"	INTEGER NOT NULL DEFAULT 0,
	"magic_rank"	INTEGER NOT NULL DEFAULT 0,
	"cooking_rank"	INTEGER NOT NULL DEFAULT 0,
	"woodcutting_rank"	INTEGER NOT NULL DEFAULT 0,
	"fishing_rank"	INTEGER NOT NULL DEFAULT 0,
	"crafting_rank"	INTEGER NOT NULL DEFAULT 0,
	"smithing_rank"	INTEGER NOT NULL DEFAULT 0,
	"mining_rank"	INTEGER NOT NULL DEFAULT 0,
	"total_rank"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "players_variables" (
	"player_id"	INTEGER NOT NULL,
	"key"	TEXT NOT NULL,
	"value"	INTEGER NOT NULL DEFAULT 0,
	PRIMARY KEY("player_id","key"),
	FOREIGN KEY("player_id") REFERENCES "players"("id")
);
CREATE TABLE IF NOT EXISTS "players_contacts" (
	"player_id"	INTEGER NOT NULL,
	"is_ignore"	INTEGER NOT NULL DEFAULT 0,
	"slot"	INTEGER NOT NULL,
	"contact"	TEXT NOT NULL,
	FOREIGN KEY("player_id") REFERENCES "players"("id"),
	PRIMARY KEY("player_id","is_ignore","slot"),
	UNIQUE("player_id","contact")
);
CREATE TABLE IF NOT EXISTS "players_inventory" (
	"player_id"	INTEGER NOT NULL,
	"slot"	INTEGER NOT NULL,
	"item_id"	INTEGER NOT NULL,
	"amount"	INTEGER NOT NULL,
	FOREIGN KEY("player_id") REFERENCES "players"("id"),
	PRIMARY KEY("player_id","slot")
);
CREATE TABLE IF NOT EXISTS "players_bank" (
	"player_id"	INTEGER NOT NULL,
	"slot"	INTEGER,
	"item_id"	INTEGER NOT NULL,
	"amount"	INTEGER NOT NULL,
	PRIMARY KEY("player_id","item_id"),
	FOREIGN KEY("player_id") REFERENCES "players"("id"),
	UNIQUE("player_id","slot")
);
CREATE INDEX IF NOT EXISTS "player_username" ON "players" (
	"username"
);
CREATE INDEX IF NOT EXISTS "player_id_inventory" ON "players_inventory" (
	"player_id"
);
CREATE INDEX IF NOT EXISTS "player_id_bank" ON "players_bank" (
	"player_id"
);
CREATE INDEX IF NOT EXISTS "player_id_variables" ON "players_variables" (
	"player_id"
);
CREATE INDEX IF NOT EXISTS "player_id_contacts" ON "players_contacts" (
	"player_id"
);
COMMIT;
