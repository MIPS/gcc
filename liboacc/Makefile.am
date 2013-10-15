ACLOCAL_AMFLAGS = -I .. -I ../config

## May be used by toolexeclibdir.
gcc_version := $(shell cat $(top_srcdir)/../gcc/BASE-VER)

config_path = @config_path@
search_path = $(addprefix $(top_srcdir)/config/, $(config_path)) $(top_srcdir)

fincludedir = $(libdir)/gcc/$(target_alias)/$(gcc_version)/finclude
libsubincludedir = $(libdir)/gcc/$(target_alias)/$(gcc_version)/include

vpath % $(strip $(search_path))

AM_CPPFLAGS = $(addprefix -I, $(search_path))
AM_CFLAGS = $(XCFLAGS)
AM_LDFLAGS = $(XLDFLAGS) $(SECTION_LDFLAGS) $(OPT_LDFLAGS)

toolexeclib_LTLIBRARIES = liboacc.la
nodist_toolexeclib_HEADERS = liboacc.spec

liboacc_la_SOURCES = liboacc.h \
					liboacc-internal.h \
					openacc.h \
					liboacc.c \
					openacc.c \
					platform.c \
					runtime.c \
					program.c \
					memory.c \
					synchro.c \
					openacc_lib.h \
					openacc_lib.f90 \
					fortran.c
					
liboacc_la_CPPFLAGS = -I$(openclinc)
