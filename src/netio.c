#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "server.h"
#include "netio.h"

static int net_set_flags(int);

int
net_establish_listener(struct server *s, int *sockets)
{
	int numsocks = 0;
	struct addrinfo hints = {0};
	struct addrinfo *ai = NULL, *ai0 = NULL;
	char portstr[32];

	(void)snprintf(portstr, sizeof(portstr), "%d", s->port);

	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(s->bind_addr, portstr, &hints, &ai0) == -1) {
		fprintf(stderr, "failed to getaddrinfo: %s\n", strerror(errno));
		goto err;
	}

	for (ai = ai0; ai != NULL; ai = ai->ai_next) {
		int s = socket(ai->ai_family, SOCK_STREAM, 0);
		if (s == -1) {
			fprintf(stderr,
			    "failed to open socket: %s\n", strerror(errno));
			continue;
		}

		if (net_set_flags(s) == -1) {
			close(s);
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
			p->outbuf_written += len;
		}
	}

	if (p->outbuf_written >= p->outbuf_len) {
		p->outbuf_len = 0;
		p->outbuf_written = 0;
	}
	return 0;
}

void
net_login_response(struct player *p, int response)
{
	if (p->protocol_rev < 196) {
		uint8_t b[2];

		b[0] = 0;
		b[1] = response;

		(void)send(p->sock, &b, sizeof(b), 0);
	} else {
		uint8_t b = response;

		(void)send(p->sock, &b, sizeof(b), 0);
	}
}

static int
net_set_flags(int s)
{
	int flags;

	flags = fcntl(s, F_GETFL, 0);
	if (flags == -1) {
		fprintf(stderr,
		    "failed to get flags: %s\n", strerror(errno));
		return -1;
	}

	if (fcntl(s, F_SETFL, flags | O_NONBLOCK) == -1) {
		fprintf(stderr,
		    "failed to set O_NONBLOCK: %s\n", strerror(errno));
		return -1;
	}

#ifdef TCP_NODELAY
	flags = 1;
	if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY,
			&flags, sizeof(flags)) == -1) {
		fprintf(stderr,
		    "failed to set TCP_NODELAY: %s\n", strerror(errno));
		return -1;
	}
#endif

	return 0;
}

int
net_player_accept(int sock)
{
	return net_set_flags(sock);
}
