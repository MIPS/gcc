## Makefile for the toplevel directory of the mudflap library.
##
## Copyright (C) 2002
## Free Software Foundation, Inc.
##

AUTOMAKE_OPTIONS = 1.3 cygnus
MAINT_CHARSET = latin1
SUBDIRS = . testsuite

# Cross compiler and multilib support.
lib_LTLIBRARIES = libmudflap.la
include_HEADERS = mf-runtime.h

CFLAGS = -O2 -g -freorder-blocks
AM_CFLAGS = -ansi -Wall

# Use common includes from acinclude.m4/LIBMUDFLAP_EXPORT_INCLUDES
LIBMUDFLAP_INCLUDES =
TOPLEVEL_INCLUDES =

mf-runtime.o: mf-runtime.h mf-impl.h
mf-hooks.o: mf-runtime.h mf-impl.h
mf-heuristics.o: mf-runtime.h mf-impl.h

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


.NOTPARALLEL:
$(HOOKOBJS): mf-hooks.c mf-runtime.h mf-impl.h
	set -e; \
	for i in $(HOOKOBJS); do \
	     hook=`basename $$i -hook.lo`; \
	     $(LTCOMPILE) -DWRAP_$$hook -c $(srcdir)/mf-hooks.c -o $$i; \
	done

libmudflap_la_LDFLAGS = 
libmudflap_la_LIBADD = $(HOOKOBJS)
libmudflap_la_DEPENDENCIES = $(libmudflap_la_LIBADD)

