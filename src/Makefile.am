AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS =	rscsundae

AM_CPPFLAGS = -Iext/libjag/include

rscsundae_SOURCES = \
	buffer.c \
	loop-ev.c \
	mob.c \
	netio.c \
	player.c \
	server.c \
	ext/flea.c \
	ext/libjag/lib/jag_bzip2_decompress.c \
	ext/libjag/lib/jag_find_entry.c \
	ext/libjag/lib/jag_hash_entry_name.c  \
	ext/libjag/lib/jag_read_entry.c \
	ext/libjag/lib/jag_read_header.c \
	ext/libjag/lib/jag_unpack_entry.c \
	ext/libjag/lib/jag_unpack_file.c \
	protocol/incoming.c \
	protocol/outgoing.c \
	protocol/utility.c

rscsundae_LDADD = $(libev_LIBS)
