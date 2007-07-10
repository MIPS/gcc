
# Copyright (C) 2007 Free Software Foundation, Inc.

AUTOMAKE_OPTIONS = 1.9.5 foreign
ACLOCAL_AMFLAGS = -I .. -I ../config

# May be used by various substitution variables.
gcc_version := $(shell cat $(top_srcdir)/../gcc/BASE-VER)

toolexeclib_LTLIBRARIES = libcilsupport.la

libcilsupport_la_SOURCES = \
	gcc4net-support.c \
	runtime-support.c
