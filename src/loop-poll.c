#include "../config.h"
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/socket.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "entity.h"
#include "loop.h"
#include "server.h"
#include "netio.h"

struct server *serv;

static void server_sock_cb(int fd);
static uint64_t get_time_ms(void);

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

static uint64_t
get_time_ms(void)
{
	uint64_t milliseconds = 0;
#ifdef HAVE_CLOCK_GETTIME
	struct timespec ts = {0};

	(void)clock_gettime(CLOCK_MONOTONIC, &ts);
	milliseconds = ts.tv_sec * 1000LL;
	milliseconds += ts.tv_nsec / 1000000LL;
#else
	struct timeval tv = {0};

	(void)gettimeofday(&tv, NULL);
	milliseconds = tv.tv_sec * 1000LL;
	milliseconds += tv.tv_usec / 1000LL;
#endif
	return milliseconds;
}

int
loop_add_player(struct player *p)
{
	(void)p;
	return 0;
}

int
loop_start(struct server *s, int port)
{
	int sockets[8];
	struct pollfd pfds[8];
	int numsockets = 0;
	uint64_t next_tick = 0;

	serv = s;

	numsockets = net_establish_listener(sockets, port);
	if (numsockets == 0) {
		fprintf(stderr, "failed to open sockets: %s\n", strerror(errno));
		goto err;
	}

	printf("got %d sockets\n", numsockets);
	for (int i = 0; i < numsockets; ++i) {
		pfds[i].fd = sockets[i];
		pfds[i].events = POLLRDNORM | POLLRDBAND;
	}

	while (poll(pfds, numsockets, 640) != -1) {
		for (int i = 0; i < numsockets; ++i) {
			if ((pfds[i].revents & POLLRDNORM) ||
			    (pfds[i].revents & POLLRDBAND)) {
				server_sock_cb(pfds[i].fd);
			}
		}
		if (get_time_ms() >= next_tick) {
			server_tick();
			next_tick = get_time_ms() + 640;
		}
	}
	for (int i = 0; i < numsockets; ++i) {
		close(sockets[i]);
	}
	printf("done\n");
	return 0;
err:
	for (int i = 0; i < numsockets; ++i) {
		close(sockets[i]);
	}
	return -1;
}
