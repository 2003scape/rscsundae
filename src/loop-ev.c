#include <ev.h>
#ifndef _WIN32
#include <sys/socket.h>
#endif
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "entity.h"
#include "loop.h"
#include "server.h"
#include "netio.h"

#ifdef _WIN32
#include "platform/win32_compat.h"
#endif

struct server *serv;

static struct ev_loop *loop;
static ev_timer tick_timer;

void
tick_cb(EV_P_ ev_timer *w, int revents)
{
	(void)w;
	(void)revents;
	server_tick();
}

void
server_sock_cb(EV_P_ ev_io *w, int revents)
{
	struct sockaddr_storage client_addr;
	socklen_t client_len = sizeof(client_addr);

	(void)revents;

	int client_sock = accept(w->fd,
	    (struct sockaddr *)&client_addr, &client_len);
	if (client_sock != -1) {
		if (net_player_accept(client_sock) == -1) {
			close(client_sock);
			return;
		}
		if (player_create(serv, client_sock) == NULL) {
			close(client_sock);
		}
	}
}

void
loop_set_delay(int delay)
{
	tick_timer.repeat = delay / 1000.0;
	ev_timer_again(loop, &tick_timer);
}

void
client_sock_cb(EV_P_ ev_io *w, int revents)
{
	struct player *p = NULL;

	for (int i = 0; i < serv->max_player_id; ++i) {
		if (serv->players[i] == NULL) {
			continue;
		}
		if (w->fd == serv->players[i]->sock) {
			p = serv->players[i];
			printf("found player\n");
			break;
		}
	}

	if (p == NULL) {
		printf("null player\n");
		return;
	}

	if (revents & EV_READ) {
		printf("recv possible\n");
		net_player_recv(p);
	}

	if (revents & EV_WRITE) {
		printf("send possible\n");
		net_player_send(p);
	}
}

int
loop_add_player(struct player *p)
{
	ev_io event;

	(void)p;
#if 0
	ev_io_init(&event, client_sock_cb, p->sock, EV_READ | EV_WRITE);
	ev_io_start(serv->loop_ctx, &event);
#endif

	return 0;
}

int
loop_start(struct server *s)
{
	int sockets[8];
	ev_io sock_events[8];
	int numsockets = 0;

	loop = EV_DEFAULT;
	s->loop_ctx = loop;

	serv = s;

	numsockets = net_establish_listener(s, sockets);
	if (numsockets == 0) {
		fprintf(stderr, "failed to open sockets: %s\n", strerror(errno));
		return -1;
	}

	ev_init(&tick_timer, tick_cb);
	tick_timer.repeat = TICK_RATE / 1000.0;
	ev_timer_again(loop, &tick_timer);

	printf("got %d sockets\n", numsockets);
	for (int i = 0; i < numsockets; ++i) {
		ev_io_init(&sock_events[i],
		    server_sock_cb, sockets[i], EV_READ);
		ev_io_start(loop, &sock_events[i]);
	}
	ev_run(loop, 0);
	printf("done\n");
	return 0;
}
