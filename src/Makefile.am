AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS =	rscsundae

rscsundae_SOURCES = \
	buffer.c \
	loop-ev.c \
	mob.c \
	netio.c \
	player.c \
	server.c \
	ext/flea.c \
	protocol/incoming.c \
	protocol/outgoing.c \
	protocol/utility.c

rscsundae_LDADD = $(libev_LIBS)
