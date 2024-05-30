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
AX_LIB_EV
AC_OUTPUT
