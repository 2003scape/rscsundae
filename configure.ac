AC_INIT([rscsundae], [0.0.1], [stormy@mailfence.com])
AM_INIT_AUTOMAKE([-Wall foreign])
: ${CFLAGS="-std=c99 -Wall"}
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_CHECK_FUNCS([arc4random_buf])
AC_CHECK_HEADER([bzlib.h])
AC_SEARCH_LIBS([BZ2_bzDecompressInit], [bz2])
AM_SILENT_RULES([yes])

AC_ARG_WITH([kqueue], 
	[AS_HELP_STRING([--with-kqueue],
		[use kqueue for the event loop (best on BSD systems) @<:@default=check@:>@])],
	[],
	[with_kqueue=check])

AC_ARG_WITH([linuxpoll], 
	[AS_HELP_STRING([--with-linuxpoll],
		[use poll+timerfd for the event loop (best on Linux) @<:@default=check@:>@])],
	[],
	[with_linuxpoll=check])

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
		[AC_MSG_FAILURE(
			[--with-kqueue was given, but test for kqueue failed])],
	)])

AM_CONDITIONAL(LINUXPOLL, false)
AS_IF([test "x$with_linuxpoll" != xno],
	[AC_CHECK_FUNCS([poll timerfd_create],
 		[
			AM_CONDITIONAL(LINUXPOLL, true)
			AC_DEFINE([HAVE_LINUXPOLL], [1], [Define if you have Linux poll])
		],
		[AC_MSG_FAILURE(
			[--with-linuxpoll was given, but test for Linux poll failed])],
	)])

AS_IF([test "x$with_libev" = xyes],
	[AC_CHECK_HEADER([ev.h],
		[AC_SEARCH_LIBS([ev_run], [ev],
			[AC_DEFINE([HAVE_LIBEV], [1], [Define if you have libev])],
			[AC_MSG_FAILURE(
				[--with-libev was given, but test for libev failed])],
		)],
		[AC_MSG_FAILURE(
			[--with-libev was given, but test for libev failed])],
	)])

AC_OUTPUT
