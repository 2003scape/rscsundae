AC_INIT([rscsundae], [0.0.1], [stormy@mailfence.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])


CFLAGS="$CFLAGS -Wall"
CFLAGS="$CFLAGS -Wextra"
CFLAGS="$CFLAGS -std=gnu99"
AC_CHECK_FUNCS([arc4random_buf])
AC_CHECK_FUNCS([clock_gettime])
AC_CHECK_FUNCS([reallocarr])
AC_CHECK_FUNCS([timerfd_create])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([sys/timerfd.h])
AC_CHECK_HEADERS([bzlib.h])
AC_SEARCH_LIBS([BZ2_bzDecompressInit], [bz2])
PKG_CHECK_MODULES([LUA], [lua-5.4])
AM_SILENT_RULES([yes])

AC_ARG_WITH([kqueue], 
	[AS_HELP_STRING([--with-kqueue],
		[use kqueue for the event loop (best on BSD systems) @<:@default=check@:>@])],
	[],
	[with_kqueue=check])

AC_ARG_WITH([libev], 
	[AS_HELP_STRING([--with-libev],
		[use libev for the event loop @<:@default=no@:>@])],
	[],
	[with_libev=check])

AM_CONDITIONAL(KQUEUE, false)
AS_IF([test "x$with_kqueue" != xno],
	[AC_CHECK_FUNCS([kqueue],
 		[
			AM_CONDITIONAL(KQUEUE, true)
			AC_DEFINE([HAVE_KQUEUE], [1], [Define if you have kqueue])
		],
		[],
	)])

AM_CONDITIONAL(LIBEV, false)
AS_IF([test "x$with_libev" = xyes],
	[AC_CHECK_HEADER([ev.h],
		[AC_SEARCH_LIBS([ev_run], [ev],
			[
				AM_CONDITIONAL(LIBEV, true)
				AC_DEFINE([HAVE_LIBEV], [1], [Define if you have libev])
			],
			[AC_MSG_FAILURE(
				[--with-libev was given, but test for libev failed])],
		)],
		[AC_MSG_FAILURE(
			[--with-libev was given, but test for libev failed])],
	)])

AC_OUTPUT
