#include <sys/event.h>
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

static struct kevent events[12];
static void server_sock_cb(int fd);

static void
server_sock_cb(int fd)
{
	struct sockaddr_storage client_addr;
	socklen_t client_len = sizeof(client_addr);

	int client_sock = accept(fd,
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

int
loop_add_player(struct player *p)
{
	(void)p;
	return 0;
}

void
loop_set_delay(int delay)
{
	EV_SET(&events[0], 0, EVFILT_TIMER,
	    EV_ADD | EV_ENABLE, NOTE_MSECONDS, delay, 0);
}

int
loop_start(struct server *s, int port)
{
	int sockets[8];
	struct kevent revents[12];
	int numsockets = 0;
	int kq = -1;

	serv = s;

	kq = kqueue();
	if (kq == 0) {
		fprintf(stderr, "failed to open kqueue: %s\n", strerror(errno));
		return -1;
	}

	numsockets = net_establish_listener(sockets, port);
	if (numsockets == 0) {
		fprintf(stderr, "failed to open sockets: %s\n", strerror(errno));
		return -1;
	}

	EV_SET(&events[0], 0, EVFILT_TIMER,
	    EV_ADD | EV_ENABLE, NOTE_MSECONDS, 640, 0);

	printf("got %d sockets\n", numsockets);
	for (int i = 0; i < numsockets; ++i) {
		EV_SET(&events[i + 1], sockets[i], EVFILT_READ,
		    EV_ADD | EV_ENABLE, 0, -1, 1);
	}

	for (;;) {
		int nev;

		if ((nev = kevent(kq, events, numsockets + 1,
		    revents, numsockets + 1, NULL)) == -1) {
			fprintf(stderr,
			    "kevent failure: %s\n", strerror(errno));
			break;
		}

		for (int i = 0; i < nev; ++i) {
			if (revents[i].udata == 0) {
				server_tick();
				continue;
			}
			server_sock_cb((int)revents[i].ident);
		}
	}
	close(kq);
	for (int i = 0; i < numsockets; ++i) {
		close(sockets[i]);
	}
	printf("done\n");
	return 0;
}
