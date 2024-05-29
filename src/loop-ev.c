#include <ev.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "entity.h"
#include "loop.h"
#include "server.h"
#include "netio.h"

struct server *serv;

void
tick_cb(EV_P_ ev_timer *w, int revents)
{
	server_tick();
}

void
server_sock_cb(EV_P_ ev_io *w, int revents)
{
	struct sockaddr_storage client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_sock = accept(w->fd,
	    (struct sockaddr *)&client_addr, &client_len);
	if (client_sock != -1) {
		int flags = fcntl(client_sock, F_GETFL, 0);
		if (flags == -1) {
			close(client_sock);
			return;
		}

		(void)fcntl(client_sock, F_SETFL, flags | O_NONBLOCK);
		if (player_accept(serv, client_sock) == NULL) {
			close(client_sock);
		}
	}
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

	printf("add player\n");
	//ev_io_init(&event, client_sock_cb, p->sock, EV_READ | EV_WRITE);
	//ev_io_start(serv->loop_ctx, &event);

	return 0;
}

int
loop_start(struct server *s, int port)
{
	struct ev_loop *loop = EV_DEFAULT;
	int sockets[8];
	ev_io sock_events[8];
	int numsockets = 0;
	ev_timer tick_timer;

	s->loop_ctx = loop;

	serv = s;

	numsockets = net_establish_listener(sockets, port);
	if (numsockets == 0) {
		fprintf(stderr, "failed to open sockets: %s\n", strerror(errno));
		return -1;
	}

	ev_timer_init(&tick_timer, tick_cb, 0.64, 0.64);
	ev_timer_start(loop, &tick_timer); 
	printf("got %d sockets\n", numsockets);
	for (unsigned i = 0; i < numsockets; ++i) {
		ev_io_init(&sock_events[i],
		    server_sock_cb, sockets[i], EV_READ);
		ev_io_start(loop, &sock_events[i]);
	}
	ev_run(loop, 0);
	printf("done\n");
	return 0;
}
