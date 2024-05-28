OBJS+=	buffer.o
OBJS+=	netio.o
OBJS+=	loop-ev.o
OBJS+=	server.o
OBJS+=	flea.o
OBJS+=	player.o
OBJS+=	incoming.o
OBJS+=	outgoing.o
OBJS+=	utility.o

CFLAGS+=	-I/usr/pkg/include/ev
LDFLAGS+=	-L/usr/pkg/lib/ev
LDFLAGS+=	-Wl,-R/usr/pkg/lib/ev

CFLAGS+=	-Wall
CFLAGS+=	-g -Og -fsanitize=undefined
LDFLAGS+=	-fsanitize=undefined

all: rscsundae

.c.o:
	$(CC) $(CFLAGS) -c $<

flea.o: ext/flea.c

utility.o: protocol/utility.c

incoming.o: protocol/incoming.c

outgoing.o: protocol/outgoing.c

rscsundae: $(OBJS)
	$(CC) $(LDFLAGS) -lev -o rscsundae $(OBJS)

clean:
	rm -f $(OBJS)
	rm -f rscsundae
