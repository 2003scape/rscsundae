#include <sys/timerfd.h>
#include <sys/socket.h>
#include <poll.h>
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

/* i'd rather have kqueue, but... */

struct server *serv;

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
	return 0;
}

int
loop_start(struct server *s, int port)
{
	int sockets[8];
	int tfd = -1;
	struct pollfd pfds[12];
	struct itimerspec its = {0};
	int numsockets = 0;

	serv = s;

	tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);

	if (tfd == 0) {
		fprintf(stderr, "failed to open timerfd: %s\n", strerror(errno));
		goto err;
	}

	its.it_value.tv_nsec = 640000000L;
	its.it_interval.tv_nsec = 640000000L;

	if (timerfd_settime(tfd, 0, &its, NULL) == -1) {
		fprintf(stderr, "failed to set timerfd: %s\n", strerror(errno));
		goto err;
	}

	pfds[0].fd = tfd;
	pfds[0].events = POLLRDNORM | POLLRDBAND;

	numsockets = net_establish_listener(sockets, port);
	if (numsockets == 0) {
		fprintf(stderr, "failed to open sockets: %s\n", strerror(errno));
		goto err;
	}

	printf("got %d sockets\n", numsockets);
	for (unsigned i = 0; i < numsockets; ++i) {
		pfds[i + 1].fd = sockets[i];
		pfds[i + 1].events = POLLRDNORM | POLLRDBAND;
	}


	while (poll(pfds, numsockets + 1, -1) != -1) {
		if ((pfds[0].revents & POLLRDNORM) ||
		    (pfds[0].revents & POLLRDBAND)) {
			(void)timerfd_settime(tfd, 0, &its, NULL);
			server_tick();
			continue;
		}
		for (unsigned i = 0; i < numsockets; ++i) {
			if ((pfds[i + 1].revents & POLLRDNORM) ||
			    (pfds[i + 1].revents & POLLRDBAND)) {
				server_sock_cb(pfds[i + 1].fd);
			}
		}
	}
	close(tfd);
	for (unsigned i = 0; i < numsockets; ++i) {
		close(sockets[i]);
	}
	printf("done\n");
	return 0;
err:
	if (tfd != -1) {
		close(tfd);
	}
	for (unsigned i = 0; i < numsockets; ++i) {
		close(sockets[i]);
	}
	return -1;
}
