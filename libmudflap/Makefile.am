## Makefile for the toplevel directory of the mudflap library.
##
## Copyright (C) 2002
## Free Software Foundation, Inc.
##

AUTOMAKE_OPTIONS = 1.3 cygnus
MAINT_CHARSET = latin1

# SUBDIRS = 
# PWD = $${PWDCMD-pwd}

# Cross compiler and multilib support.
lib_LTLIBRARIES = libmudflap.la

# Compile flags that should be constant throughout the build, both for
# SUBDIRS and for libmudflap in general.
CFLAGS = -O2 -g
WARN_CFLAGS = $(WERROR) -fdiagnostics-show-location=once

# Use common includes from acinclude.m4/LIBMUDFLAP_EXPORT_INCLUDES
LIBMUDFLAP_INCLUDES =
TOPLEVEL_INCLUDES =

MFCONFIG_CFLAGS = -I $(srcdir) -include mf-config.h

libmudflap_la_SOURCES = \
	mf-runtime.c \
	mf-hooks.c
INCLUDES = \
	$(MFCONFIG_CFLAGS)

# libmudflap_la_LIBADD =
# libmudflap_la_LDFLAGS = 

libmudflap_la_DEPENDENCIES = $(libmudflap_la_LIBADD)

# COMPILE = $(LIBTOOL) --tag CC --mode=compile $(CC) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(CFLAGS) $(AM_CFLAGS) $(WARN_CFLAGS) $(MFCONFIG_CFLAGS)
# LINK = $(LIBTOOL) --tag CC --mode=link $(CC) $(AM_CFLAGS) $(LDFLAGS) -o $@
