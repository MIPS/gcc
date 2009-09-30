# Makefile.am is used by automake 1.9.6 to generate Makefile.in.

gcc_version := $(shell cat $(top_srcdir)/../gcc/BASE-VER)
target_noncanonical := @target_noncanonical@
libexecsubdir := $(libexecdir)/gcc/$(target_noncanonical)/$(gcc_version)

# How to find libelf
LIBELFLIBS = @LIBELFLIBS@
LIBELFINC = @LIBELFINC@

AM_CPPFLAGS = -I$(top_srcdir)/../include -D_LARGEFILE_SOURCE \
	-D_FILE_OFFSET_BITS=64 $(LIBELFINC)

AM_CFLAGS = -Wall -Werror

libexecsub_LTLIBRARIES = liblto_plugin.la

liblto_plugin_la_SOURCES = lto-plugin.c $(top_srcdir)/../gcc/lto/common.c
liblto_plugin_la_LIBADD = $(LIBELFLIBS) -L../libiberty/pic -liberty
