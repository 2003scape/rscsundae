#include "opcodes.h"

uint8_t opcodes_in_203[256];
uint8_t opcodes_out_203[256];

void
init_opcodes_203(void)
{
	opcodes_in_203[0] = OP_CLI_LOGIN;
	opcodes_in_203[2] = OP_CLI_REGISTER;
	opcodes_in_203[216] = OP_CLI_PUBLIC_CHAT;
	opcodes_in_203[67] = OP_CLI_PING;
	opcodes_in_203[102] = OP_CLI_LOGOUT;
	opcodes_in_203[38] = OP_CLI_ADMIN_COMMAND;
	opcodes_in_203[1] = OP_CLI_RECONNECT;
	opcodes_in_203[195] = OP_CLI_ADD_FRIEND;
	opcodes_in_203[167] = OP_CLI_REMOVE_FRIEND;
	opcodes_in_203[218] = OP_CLI_PRIVATE_MESSAGE;
	opcodes_in_203[132] = OP_CLI_ADD_IGNORE;
	opcodes_in_203[241] = OP_CLI_REMOVE_IGNORE;
	opcodes_in_203[64] = OP_CLI_PRIVACY_SETTINGS;
	opcodes_in_203[104] = OP_CLI_TRADE_CONFIRM;
	opcodes_in_203[23] = OP_CLI_BANK_DEPOSIT;
	opcodes_in_203[22] = OP_CLI_BANK_WITHDRAW;
	opcodes_in_203[212] = OP_CLI_BANK_CLOSE;
	opcodes_in_203[254] = OP_CLI_PRAYER_OFF;
	opcodes_in_203[60] = OP_CLI_PRAYER_ON;
	opcodes_in_203[111] = OP_CLI_SAVE_SETTING;
	opcodes_in_203[165] = OP_CLI_FOLLOW_PLAYER;
	opcodes_in_203[16] = OP_CLI_WALK_ENTITY;
	opcodes_in_203[221] = OP_CLI_SHOP_SELL;
	opcodes_in_203[236] = OP_CLI_SHOP_BUY;
	opcodes_in_203[166] = OP_CLI_SHOP_CLOSE;
	opcodes_in_203[113] = OP_CLI_PLAYER_USEWITH;
	opcodes_in_203[4] = OP_CLI_INV_CAST;
	opcodes_in_203[99] = OP_CLI_LOC_CAST;
	opcodes_in_203[180] = OP_CLI_BOUND_CAST;
	opcodes_in_203[249] = OP_CLI_ITEM_CAST;
	opcodes_in_203[50] = OP_CLI_NPC_CAST;
	opcodes_in_203[229] = OP_CLI_PLAYER_CAST;
	opcodes_in_203[137] = OP_CLI_SELF_CAST;
	opcodes_in_203[171] = OP_CLI_ATTACK_PLAYER;
	opcodes_in_203[127] = OP_CLI_BOUND_OP2;
	opcodes_in_203[79] = OP_CLI_LOC_OP2;
	opcodes_in_203[29] = OP_CLI_COMBAT_STYLE;
	opcodes_in_203[55] = OP_CLI_TRADE_ACCEPT;
	opcodes_in_203[230] = OP_CLI_TRADE_DECLINE;
	opcodes_in_203[46] = OP_CLI_TRADE_UPDATE;
	opcodes_in_203[142] = OP_CLI_TRADE_PLAYER;
	opcodes_in_203[235] = OP_CLI_ACCEPT_DESIGN;
	opcodes_in_203[116] = OP_CLI_ANSWER_MULTI;
	opcodes_in_203[14] = OP_CLI_BOUND_OP1;
	opcodes_in_203[161] = OP_CLI_BOUND_USEWITH;
	opcodes_in_203[91] = OP_CLI_INV_USEWITH;
	opcodes_in_203[115] = OP_CLI_LOC_USEWITH;
	opcodes_in_203[136] = OP_CLI_LOC_OP1;
	opcodes_in_203[135] = OP_CLI_NPC_USEWITH;
	opcodes_in_203[190] = OP_CLI_NPC_ATTACK;
	opcodes_in_203[153] = OP_CLI_NPC_TALK;
	opcodes_in_203[90] = OP_CLI_INV_OP1;
	opcodes_in_203[170] = OP_CLI_INV_UNWEAR;
	opcodes_in_203[169] = OP_CLI_INV_WEAR;
	opcodes_in_203[53] = OP_CLI_ITEM_USEWITH;
	opcodes_in_203[246] = OP_CLI_INV_DROP;
	opcodes_in_203[247] = OP_CLI_ITEM_TAKE;
	opcodes_in_203[187] = OP_CLI_WALK_TILE;

	opcodes_out_203[OP_SRV_MESSAGE] = 131;
	opcodes_out_203[OP_SRV_LOGOUT] = 4;
	opcodes_out_203[OP_SRV_LOGOUT_REJECT] = 183;
	opcodes_out_203[OP_SRV_INIT_FRIENDS] = 71;
	opcodes_out_203[OP_SRV_FRIEND_UPDATE] = 149;
	opcodes_out_203[OP_SRV_INIT_IGNORE] = 109;
	opcodes_out_203[OP_SRV_PRIVACY_SETTINGS] = 51;
	opcodes_out_203[OP_SRV_PRIVATE_MESSAGE] = 120;
	opcodes_out_203[OP_SRV_STAT] = 159;
	opcodes_out_203[OP_SRV_INVENTORY_REMOVE] = 123;
	opcodes_out_203[OP_SRV_INVENTORY_ITEM] = 90;
	opcodes_out_203[OP_SRV_TRADE_CONFIRM] = 20;
	opcodes_out_203[OP_SRV_STAT_XP] = 33;
	opcodes_out_203[OP_SRV_BANK_CLOSE] = 203;
	opcodes_out_203[OP_SRV_BANK_SHOW] = 42;
	opcodes_out_203[OP_SRV_QUESTS] = 5;
	opcodes_out_203[OP_SRV_PRAYERS] = 206;
	opcodes_out_203[OP_SRV_CLIENT_SETTINGS] = 240;
	opcodes_out_203[OP_SRV_TRADE_STATE_LOCAL] = 0;
	opcodes_out_203[OP_SRV_SHOP_CLOSE] = 137;
	opcodes_out_203[OP_SRV_SHOP_OPEN] = 101;
	opcodes_out_203[OP_SRV_TRADE_STATE_REMOTE] = 162;
	opcodes_out_203[OP_SRV_UPDATE_TRADE_OFFER] = 97;
	opcodes_out_203[OP_SRV_CLOSE_TRADE] = 128;
	opcodes_out_203[OP_SRV_SHOW_TRADE] = 92;
	opcodes_out_203[OP_SRV_SHOW_DESIGN] = 59;
	opcodes_out_203[OP_SRV_DEATH] = 83;
	opcodes_out_203[OP_SRV_EQUIP_BONUSES] = 153;
	opcodes_out_203[OP_SRV_INIT_STATS] = 156;
	opcodes_out_203[OP_SRV_INIT_PLANE] = 25;
	opcodes_out_203[OP_SRV_HIDE_MULTI] = 252;
	opcodes_out_203[OP_SRV_SHOW_MULTI] = 245;
	opcodes_out_203[OP_SRV_NPC_APPEARANCE] = 104;
	opcodes_out_203[OP_SRV_NPC_MOVEMENT] = 79;
	opcodes_out_203[OP_SRV_BOUNDARIES] = 91;
	opcodes_out_203[OP_SRV_PLAYER_APPEARANCE] = 234;
	opcodes_out_203[OP_SRV_INVENTORY] = 53;
	opcodes_out_203[OP_SRV_LOCS] = 48;
	opcodes_out_203[OP_SRV_GROUND_ITEMS] = 99;
	opcodes_out_203[OP_SRV_PLAYER_MOVEMENT] = 191;
	opcodes_out_203[OP_SRV_SHOW_EFFECT] = 36;
}
