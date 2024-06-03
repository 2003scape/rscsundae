AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS =	rscsundae

AM_CPPFLAGS = -Iext/libjag/include

rscsundae_SOURCES = \
	buffer.c \
	mob.c \
	netio.c \
	player.c \
	inventory.c \
	server.c \
	stat.c \
	zone.c \
	config/config.c \
	config/text.c \
	ext/flea.c \
	ext/libjag/lib/jag_bzip2_decompress.c \
	ext/libjag/lib/jag_find_entry.c \
	ext/libjag/lib/jag_hash_entry_name.c  \
	ext/libjag/lib/jag_map_read_jm.c \
	ext/libjag/lib/jag_read_entry.c \
	ext/libjag/lib/jag_read_header.c \
	ext/libjag/lib/jag_unpack_entry.c \
	ext/libjag/lib/jag_unpack_file.c \
	ext/reallocarr.c \
	protocol/incoming.c \
	protocol/outgoing.c \
	protocol/utility.c

if KQUEUE
rscsundae_SOURCES += loop-kqueue.c
else
if LIBEV
rscsundae_SOURCES += loop-ev.c
else
rscsundae_SOURCES += loop-poll.c
endif
endif

rscsundae_LDADD = $(libev_LIBS) -lm
