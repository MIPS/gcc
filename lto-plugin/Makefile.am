AM_CPPFLAGS = -I$(top_srcdir)/../include

AM_CFLAGS = -Wall -Werror

bin_PROGRAMS = ltosymtab

ltosymtab_SOURCES = lto-symtab.c
ltosymtab_LDADD = -ldl

lib_LTLIBRARIES = liblto_plugin.la

liblto_plugin_la_SOURCES = lto-plugin.c
liblto_plugin_la_LIBADD = -lelf
