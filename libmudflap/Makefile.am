## Makefile for the toplevel directory of the mudflap library.
##
## Copyright (C) 2002, 2003
## Free Software Foundation, Inc.
##

AUTOMAKE_OPTIONS = 1.3 foreign
MAINT_CHARSET = latin1
SUBDIRS = testsuite

AM_CFLAGS = -ansi -Wall -O3 -g -freorder-blocks

lib_LTLIBRARIES = libmudflap.la
include_HEADERS = mf-runtime.h

mf-runtime.h: mf-runtime.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

libmudflap_la_SOURCES = \
	mf-runtime.c \
	mf-heuristics.c

HOOKOBJS = \
 malloc-hook.lo \
 free-hook.lo \
 calloc-hook.lo \
 realloc-hook.lo \
 memcpy-hook.lo \
 memmove-hook.lo \
 memset-hook.lo \
 memcmp-hook.lo \
 memchr-hook.lo \
 memrchr-hook.lo \
 strcpy-hook.lo \
 strncpy-hook.lo \
 strcat-hook.lo \
 strncat-hook.lo \
 strcmp-hook.lo \
 strcasecmp-hook.lo \
 strncmp-hook.lo \
 strncasecmp-hook.lo \
 strdup-hook.lo \
 strndup-hook.lo \
 strchr-hook.lo \
 strrchr-hook.lo \
 strstr-hook.lo \
 memmem-hook.lo \
 strlen-hook.lo \
 strnlen-hook.lo \
 bzero-hook.lo \
 bcopy-hook.lo \
 bcmp-hook.lo \
 index-hook.lo \
 rindex-hook.lo \
 dlopen-hook.lo \
 mmap-hook.lo \
 munmap-hook.lo \
 alloca-hook.lo

#.NOTPARALLEL: # Otherwise the following loop will be executed in parallel!
$(HOOKOBJS): mf-hooks.c mf-runtime.h mf-impl.h
	hook=`basename $@ -hook.lo`; \
	$(LTCOMPILE) -DWRAP_$$hook -c $(srcdir)/mf-hooks.c -o $@

libmudflap_la_LDFLAGS = 
libmudflap_la_LIBADD = $(HOOKOBJS)
libmudflap_la_DEPENDENCIES = $(libmudflap_la_LIBADD)


# XXX hack alert
# From libffi/Makefile.am

# Work around what appears to be a GNU make bug handling MAKEFLAGS
# values defined in terms of make variables, as is the case for CC and
# friends when we are called from the top level Makefile.
AM_MAKEFLAGS = \
	"AR_FLAGS=$(AR_FLAGS)" \
	"CC_FOR_BUILD=$(CC_FOR_BUILD)" \
	"CFLAGS=$(CFLAGS)" \
	"CXXFLAGS=$(CXXFLAGS)" \
	"CFLAGS_FOR_BUILD=$(CFLAGS_FOR_BUILD)" \
	"CFLAGS_FOR_TARGET=$(CFLAGS_FOR_TARGET)" \
	"INSTALL=$(INSTALL)" \
	"INSTALL_DATA=$(INSTALL_DATA)" \
	"INSTALL_PROGRAM=$(INSTALL_PROGRAM)" \
	"INSTALL_SCRIPT=$(INSTALL_SCRIPT)" \
	"JC1FLAGS=$(JC1FLAGS)" \
	"LDFLAGS=$(LDFLAGS)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"LIBCFLAGS_FOR_TARGET=$(LIBCFLAGS_FOR_TARGET)" \
	"MAKE=$(MAKE)" \
	"MAKEINFO=$(MAKEINFO) $(MAKEINFOFLAGS)" \
	"PICFLAG=$(PICFLAG)" \
	"PICFLAG_FOR_TARGET=$(PICFLAG_FOR_TARGET)" \
	"SHELL=$(SHELL)" \
	"exec_prefix=$(exec_prefix)" \
	"infodir=$(infodir)" \
	"libdir=$(libdir)" \
	"prefix=$(prefix)" \
	"AR=$(AR)" \
	"AS=$(AS)" \
	"CC=$(CC)" \
	"CXX=$(CXX)" \
	"LD=$(LD)" \
	"LIBCFLAGS=$(LIBCFLAGS)" \
	"NM=$(NM)" \
	"PICFLAG=$(PICFLAG)" \
	"RANLIB=$(RANLIB)" \
	"DESTDIR=$(DESTDIR)"

MAKEOVERRIDES=

# Multilib support variables.
MULTISRCTOP =
MULTIBUILDTOP =
MULTIDIRS =
MULTISUBDIR =
MULTIDO = true
MULTICLEAN = true

# Multilib support.
.PHONY: all-multi mostlyclean-multi clean-multi distclean-multi \
	maintainer-clean-multi

all-recursive: all-multi
install-recursive: install-multi
mostlyclean-recursive: mostlyclean-multi
clean-recursive: clean-multi
distclean-recursive: distclean-multi
maintainer-clean-recursive: maintainer-clean-multi

all-multi:
	: $(MAKE) ; exec $(MULTIDO) $(AM_MAKEFLAGS) DO=all multi-do
install-multi:
	$(MULTIDO) $(AM_MAKEFLAGS) DO=install multi-do
mostlyclean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=mostlyclean multi-clean
clean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=clean multi-clean
distclean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=distclean multi-clean
maintainer-clean-multi:
	$(MULTICLEAN) $(AM_MAKEFLAGS) DO=maintainer-clean multi-clean


## ################################################################

