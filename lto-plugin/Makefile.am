AM_CPPFLAGS = -I$(top_srcdir)/../include

AM_CFLAGS = -Wall -Werror

bin_PROGRAMS = ltosymtab

ltosymtab_SOURCES = lto-symtab.c $(top_srcdir)/../gcc/lto/common.c
ltosymtab_LDADD = -ldl -lelf
#hack to use common.c in both targets
ltosymtab_CFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = liblto_plugin.la

liblto_plugin_la_SOURCES = lto-plugin.c $(top_srcdir)/../gcc/lto/common.c
liblto_plugin_la_LIBADD = -lelf
