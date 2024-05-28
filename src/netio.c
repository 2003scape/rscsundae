#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "netio.h"

int
net_establish_listener(int *sockets, int port)
{
	int numsocks = 0;
	struct addrinfo hints = {0};
	struct addrinfo *ai = NULL, *ai0 = NULL;
	char portstr[32];

	(void)snprintf(portstr, sizeof(portstr), "%d", port);

	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

	if (getaddrinfo("127.0.0.1", portstr, &hints, &ai0) == -1) {
		fprintf(stderr, "failed to getaddrinfo: %s\n", strerror(errno));
		goto err;
	}

	for (ai = ai0; ai != NULL; ai = ai->ai_next) {
		int s = socket(ai->ai_family, SOCK_STREAM | SOCK_NONBLOCK, 0);
		if (s == -1) {
			fprintf(stderr,
			    "failed to open socket: %s\n", strerror(errno));
			continue;
		}

		if (bind(s, ai->ai_addr, ai->ai_addrlen) == -1) {
			fprintf(stderr,
			    "failed to bind socket: %s\n", strerror(errno));
			close(s);
			continue;
		}

		if (listen(s, SOMAXCONN) == -1) {
			fprintf(stderr,
			    "failed to listen on socket: %s\n", strerror(errno));
			close(s);
			continue;
		}

		sockets[numsocks++] = s;
	}

	freeaddrinfo(ai0);
	ai0 = NULL;

	return numsocks;
err:
	if (ai0 != NULL) {
		freeaddrinfo(ai0);
	}
	return -1;
}

int
net_player_recv(struct player *p)
{
	size_t max = PLAYER_BUFSIZE - p->inbuf_len;
	ssize_t n = recv(p->sock, p->inbuf + p->inbuf_len, max, 0);

	if (n > 0) {
		printf("read %d bytes\n", n);
		p->inbuf_len += n;
	}
	return 0;
}

int
net_player_send(struct player *p)
{
	size_t to_write = p->outbuf_len - p->outbuf_written;

	if (to_write > 0) {
		ssize_t len = send(p->sock,
			p->outbuf + p->outbuf_written, to_write, 0);
		if (len > 0) {
			printf("sent %zd bytes, off %zu\n", len, p->outbuf_written);
			p->outbuf_written += len;
		}
	}

	if (p->outbuf_written >= p->outbuf_len) {
		p->outbuf_len = 0;
		p->outbuf_written = 0;
	}
	return 0;
}
