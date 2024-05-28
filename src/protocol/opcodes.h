#ifndef OPCODES_H
#define OPCODES_H

#define OP_CLI_LOGIN		(0)
#define OP_CLI_PUBLIC_CHAT	(3)
#define OP_CLI_PING		(5)
#define OP_CLI_LOGOUT		(6)
#define OP_CLI_FOLLOW_PLAYER	(214)
#define OP_CLI_ACCEPT_DESIGN	(236)
#define OP_CLI_WALK_TILE	(255)

#define OP_SRV_MESSAGE			(8)
#define OP_SRV_LOGOUT			(9)
#define OP_SRV_SHOW_DESIGN		(239)
#define OP_SRV_STATS			(243)
#define OP_SRV_INIT_PLANE		(244)
#define OP_SRV_PLAYER_APPEARANCE	(250)
#define OP_SRV_PLAYER_MOVEMENT		(255)

#define RESP_LOGIN_OK		(0)
#define RESP_FULL		(14)
#endif
