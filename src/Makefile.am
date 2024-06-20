AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS =	rscsundae

AM_CPPFLAGS = -Iext/libjag/include
AM_CPPFLAGS += $(LUA_CFLAGS)
AM_CPPFLAGS += $(INIH_CFLAGS)
AM_CPPFLAGS += $(LIBCRYPTO_CFLAGS)

# only for testing, so slow it's unusable
#AM_CPPFLAGS += -Iext/tiny-bignum-c
#AM_CPPFLAGS += -DWITH_RSA_TINY

AM_CPPFLAGS += -DWITH_RSA_OPENSSL

rscsundae_SOURCES = \
	admin.c \
	bank.c \
	buffer.c \
	mob.c \
	netio.c \
	player.c \
	inventory.c \
	npc.c \
	script.c \
	server.c \
	settings.c \
	shop.c \
	stat.c \
	trade.c \
	utility.c \
	zone.c \
	config/config.c \
	config/text.c \
	ext/flea.c \
	ext/isaac.c \
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
	protocol/opcodes203.c \
	protocol/outgoing.c

#rscsundae_SOURCES += protocol/rsa-tiny.c
rscsundae_SOURCES += protocol/rsa-openssl.c

if ! HAVE_SYSTEM_INIH
AM_CPPFLAGS += -Iext/inih
rscsundae_SOURCES += ext/inih/ini.c
endif

if KQUEUE
rscsundae_SOURCES += loop-kqueue.c
else
if LIBEV
rscsundae_SOURCES += loop-ev.c
else
rscsundae_SOURCES += loop-poll.c
endif
endif

rscsundae_LDADD = -lm
rscsundae_LDADD += $(libev_LIBS)
rscsundae_LDADD += $(LUA_LIBS)
rscsundae_LDADD += $(INIH_LIBS)
rscsundae_LDADD += $(LIBCRYPTO_LIBS)
