#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([WormWar], [1.0], [aomdoa@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

AC_PROG_CC

PKG_CHECK_MODULES(ALLEGRO, allegro-5 allegro_main-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5)

AC_ARG_WITH([fps], [AS_HELP_STRING([--with-fps=FPS], [set the FPS for the game, default is 60])], [fps=${withval}], [fps=60])
AC_DEFINE_UNQUOTED([FPS], [$fps], [The game framerate])

AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [Turn on debugging])], [debug=true])
if test x"$debug" = x"true"; then
	AC_DEFINE([DEBUG], [], [Debug mode])
fi

AC_OUTPUT
