# Process this file with autoconf to produce a configure script.

AC_INIT(mes, [0.99], [lvecsey-mes@phrasep.com])

AC_PREREQ(2.59)

VERSION_MAJOR="0"
VERSION_MINOR="9"
VERSION_REVISION="9"
VERSION_EXTRA="git"

CONFIGURE_LINE="`echo "$0 $ac_configure_args" | sed -e 's/\\\/\\\\\\\/g'`"
CODENAME="Bilgito"

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([subdir-objects])

AC_CONFIG_HEADERS(config.h)

AC_PROG_CC
AC_PROG_MAKE_SET

AM_PROG_CC_C_O

AC_CHECK_HEADER(stdio.h,[AC_DEFINE(HAVE_STDIO_H)],[AC_MSG_ERROR([Sorry, can't do anything for you])])

AC_CONFIG_SRCDIR([FastCGI-linkshare.c])

AC_PATH_X
AC_HEADER_STDC

AC_HEADER_STDBOOL
AC_C_CONST
AC_HEADER_TIME

AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_STAT
AC_FUNC_VPRINTF

AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(malloc.h)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(arpa/inet.h)
AC_CHECK_HEADERS(intrin.h)
AC_CHECK_HEADERS(math.h)

AC_SEARCH_LIBS([FCGI_Accept], [fcgi])
AC_SEARCH_LIBS([pthread_join], [pthread])
AC_SEARCH_LIBS([uriFreeQueryListA], [uriparser])

AC_CHECK_FUNCS(qsort_r)
AC_CHECK_FUNCS(socket)
AC_CHECK_FUNCS(inet_ntoa)
AC_CHECK_FUNCS(sincos)
AC_CHECK_FUNCS(sqrt)
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(setenv)
AC_CHECK_FUNCS(memset)
AC_CHECK_FUNCS(strstr)
AC_CHECK_FUNCS(strtol)

AC_SUBST(ADDITIONAL_LDFLAGS)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT


