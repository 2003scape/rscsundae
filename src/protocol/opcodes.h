#ifndef OPCODES_H
#define OPCODES_H

#define OP_CLI_LOGIN		(0)
#define OP_CLI_PUBLIC_CHAT	(3)
#define OP_CLI_PING		(5)
#define OP_CLI_LOGOUT		(6)
#define OP_CLI_RECONNECT	(19)
#define OP_CLI_ADD_FRIEND	(26)
#define OP_CLI_REMOVE_FRIEND	(27)
#define OP_CLI_PRIVATE_MESSAGE	(28)
#define OP_CLI_ADD_IGNORE	(29)
#define OP_CLI_REMOVE_IGNORE	(30)
#define OP_CLI_PRIVACY_SETTINGS	(31)
#define OP_CLI_PRAYER_OFF	(211)
#define OP_CLI_PRAYER_ON	(212)
#define OP_CLI_SAVE_SETTING	(213)
#define OP_CLI_FOLLOW_PLAYER	(214)
#define OP_CLI_WALK_ENTITY	(215)
#define OP_CLI_ATTACK_PLAYER	(228)
#define OP_CLI_COMBAT_STYLE	(231)
#define OP_CLI_ACCEPT_DESIGN	(236)
#define OP_CLI_INV_UNWEAR	(248)
#define OP_CLI_INV_WEAR		(249)
#define OP_CLI_WALK_TILE	(255)

#define OP_SRV_MESSAGE			(8)
#define OP_SRV_LOGOUT			(9)
#define OP_SRV_LOGOUT_REJECT		(10)
#define OP_SRV_INIT_FRIENDS		(23)
#define OP_SRV_FRIEND_UPDATE		(24)
#define OP_SRV_INIT_IGNORE		(26)
#define OP_SRV_PRIVACY_SETTINGS		(27)
#define OP_SRV_PRIVATE_MESSAGE		(28)
#define OP_SRV_STAT			(211)
#define OP_SRV_INVENTORY_REMOVE		(212)
#define OP_SRV_INVENTORY_ITEM		(213)
#define OP_SRV_STAT_XP			(220)
#define OP_SRV_PRAYERS			(227)
#define OP_SRV_CLIENT_SETTINGS		(228)
#define OP_SRV_SHOW_DESIGN		(239)
#define OP_SRV_INIT_STATS		(243)
#define OP_SRV_DEATH			(241)
#define OP_SRV_EQUIP_BONUSES		(242)
#define OP_SRV_INIT_PLANE		(244)
#define OP_SRV_PLAYER_APPEARANCE	(250)
#define OP_SRV_INVENTORY		(252)
#define OP_SRV_LOCS			(253)
#define OP_SRV_PLAYER_MOVEMENT		(255)

#define RESP_LOGIN_OK		(0)
#define RESP_ACCOUNT_USED	(4)
#define RESP_FULL		(14)
#endif
