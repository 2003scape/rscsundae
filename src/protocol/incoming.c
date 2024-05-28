#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "opcodes.h"
#include "utility.h"
#include "../config/anim.h"
#include "../buffer.h"
#include "../entity.h"

#define MAX_PACKETS_PER_TICK		(10)
#define MAX_PLAYER_HAIR_COLOUR		(9)
#define MAX_PLAYER_SKIN_COLOUR		(4)
#define MAX_PLAYER_CLOTHING_COLOUR	(14)

static void
process_packet(struct player *, uint8_t *, size_t);

int
player_parse_incoming(struct player *p)
{
	uint8_t *b = NULL;
	int counter = 0;

	while (p->inbuf_processed < p->inbuf_len) {
		if (counter >= MAX_PACKETS_PER_TICK) {
			break;
		}
		if ((p->inbuf_len - p->inbuf_processed) < 2) {
			/* header not yet available */
			break;
		}
		int len = p->inbuf[p->inbuf_processed] & 0xff;
		int len2 = p->inbuf[p->inbuf_processed + 1] & 0xff;
		if (len >= 160) {
			/* "large" packets use two bytes for the length */
			len = (len - 160) * 256 + len2;
		}
		if (len == 0) {
			/* not possible through normal usage */
			printf("len == 0\n");
			break;
		}
		b = malloc(len);
		if (b == NULL) {
			break;
		}
		if (len > 1) {
			if (len < 160) {
				if ((p->inbuf_len - p->inbuf_processed - 1) < len) {
					/* insufficient data in inbuf so far... */
					break;
				}
				memcpy(b, p->inbuf + p->inbuf_processed + 2, len - 1);
				p->inbuf_processed += (len + 1);
			} else {
				if ((p->inbuf_len - p->inbuf_processed - 2) < len) {
					break;
				}
				memcpy(b, p->inbuf + p->inbuf_processed + 2, len);
				p->inbuf_processed += (len + 2);
			}
		} else {
			p->inbuf_processed += (len + 1);
		}
		/* for smaller packets len2 is the last byte */
		if (len < 160) {
			b[len - 1] = len2;
		}
		process_packet(p, b, (size_t)len);
		free(b);
		b = NULL;
		counter++;
	}
	if (p->inbuf_processed >= p->inbuf_len) {
		p->inbuf_processed = 0;
		p->inbuf_len = 0;
	}

	return 0;
}

void
process_packet(struct player *p, uint8_t *data, size_t len)
{
	size_t offset = 0;
	uint8_t opcode = data[offset++] & 0xff;

	printf("process packet opcode %d len %d\n", opcode, len);

	switch (opcode) {
	case OP_CLI_LOGIN:
		{
			char namestr[64];
			uint16_t ver;
			int64_t name;

			if (buf_getu16(data, offset, len, &ver) == -1) {
				return;
			}
			offset += 2;
			printf("got version number: %d\n", ver);

			if (buf_gets64(data, offset, len, &name) == -1) {
				return;
			}
			offset += 8;
			printf("got username: %lld %s\n", name, mod37_namedec(name, namestr));

			p->name = name;
		}
		break;
	case OP_CLI_PUBLIC_CHAT:
		{
			/* want to eventually decode this to moderate */
			size_t msglen = len - 1;
			if (msglen <= MAX_PUBLIC_CHAT_LEN) {
				memcpy(p->public_chat_enc, data + offset, msglen);
				p->public_chat_len = msglen;
			}
		}
		break;
	case OP_CLI_PING:
		{
			/* TODO */
		}
		break;
	case OP_CLI_WALK_TILE:
		{
			size_t steps;
			int start_x, start_y;

			if (len < 5) {
				return;
			}
			steps = (len - 5) / 2;
			printf("number of steps is %d\n", steps);
			if (buf_getu16(data, offset, len, &p->walk_queue_x[0]) == -1) {
				return;
			}
			offset += 2;
			if (buf_getu16(data, offset, len, &p->walk_queue_y[0]) == -1) {
				return;
			}
			offset += 2;
			start_x = p->walk_queue_x[0];
			start_y = p->walk_queue_y[0];
			if (steps > (WALK_QUEUE_LEN - 1)) {
				steps = WALK_QUEUE_LEN - 1;
			}
			for (size_t i = 0; i < steps; ++i) {
				uint8_t off_x, off_y;
				int new_x, new_y;

				if (buf_getu8(data, offset++, len, &off_x) == -1) {
					return;
				}
				if (buf_getu8(data, offset++, len, &off_y) == -1) {
					return;
				}
				new_x = start_x + (int8_t)off_x;
				new_y = start_y + (int8_t)off_y;
				p->walk_queue_x[i + 1] = (uint16_t)new_x;
				p->walk_queue_y[i + 1] = (uint16_t)new_y;
			}
			p->walk_queue_len = steps + 1;
			p->walk_queue_pos = 0 ;
		}
		break;
	case OP_CLI_ACCEPT_DESIGN:
		{
			if (!p->ui_design_open) {
				return;
			}
			(void)buf_getu8(data, offset++, len, &p->gender);
			(void)buf_getu8(data, offset++, len, &p->sprites[ANIM_SLOT_HEAD]);
			if (p->sprites[ANIM_SLOT_HEAD] != ANIM_HEAD1 &&
			    p->sprites[ANIM_SLOT_HEAD] != ANIM_HEAD2 &&
			    p->sprites[ANIM_SLOT_HEAD] != ANIM_HEAD3 &&
			    p->sprites[ANIM_SLOT_HEAD] != ANIM_HEAD4 &&
			    p->sprites[ANIM_SLOT_HEAD] != ANIM_FHEAD1) {
				p->sprites[ANIM_SLOT_HEAD] = ANIM_HEAD1;
			}
			p->sprites[ANIM_SLOT_HEAD]++;

			(void)buf_getu8(data, offset++, len, &p->sprites[ANIM_SLOT_BODY]);
			if (p->sprites[ANIM_SLOT_BODY] != ANIM_BODY1 &&
			    p->sprites[ANIM_SLOT_BODY] != ANIM_FBODY1) {
				p->sprites[ANIM_SLOT_BODY] = ANIM_BODY1;
			}
			p->sprites[ANIM_SLOT_BODY]++;

			(void)buf_getu8(data, offset++, len, &p->sprites[ANIM_SLOT_LEGS]);
			/* authentically only one choice of leg */
			if (p->sprites[ANIM_SLOT_LEGS] != ANIM_LEGS1) {
				p->sprites[ANIM_SLOT_LEGS] = ANIM_LEGS1;
			}
			p->sprites[ANIM_SLOT_LEGS]++;

			(void)buf_getu8(data, offset++, len, &p->hair_colour);
			if (p->hair_colour > MAX_PLAYER_HAIR_COLOUR) {
				p->hair_colour= 0;
			}

			(void)buf_getu8(data, offset++, len, &p->top_colour);
			if (p->top_colour > MAX_PLAYER_CLOTHING_COLOUR) {
				p->top_colour= 0;
			}

			(void)buf_getu8(data, offset++, len, &p->leg_colour);
			if (p->leg_colour > MAX_PLAYER_CLOTHING_COLOUR) {
				p->leg_colour= 0;
			}

			(void)buf_getu8(data, offset++, len, &p->skin_colour);
			if (p->skin_colour > MAX_PLAYER_SKIN_COLOUR) {
				p->skin_colour = 0;
			}

			(void)buf_getu8(data, offset++, len, &p->rpg_class);

			p->appearance_changed = true;
			p->ui_design_open = false;
		}
		break;
	}
}
