## Makefile for the toplevel directory of the mudflap library.
##
## Copyright (C) 2002
## Free Software Foundation, Inc.
##

AUTOMAKE_OPTIONS = 1.3 cygnus
MAINT_CHARSET = latin1

TESTS_ENVIRONMENT = MUDFLAP_OPTIONS='-mode-check -viol-abort'

TESTS = test/fail1.x test/fail10.x test/fail11.x test/fail12.x \
 test/fail2.x test/fail3.x test/fail4.x test/fail5.x test/fail6.x \
 test/fail7.x test/fail8.x test/fail9.x test/pass1.x test/pass10.x \
 test/pass11.x test/pass12.x test/pass2.x test/pass3.x test/pass4.x \
 test/pass5.x test/pass6.x test/pass7.x test/pass8.x test/pass9.x 


test/%.c: test/%-frag.c test/mf-driver.c test/buildtest.sh
	@mkdir -p test
	@cat $(srcdir)/test/mf-driver.c > $@
	@echo 'void test() {' >> $@
	@cat $< >> $@
	@echo '}' >> $@

test/fail%.x: test/fail%.c
	$(CC) -DSHOULD_FAIL -fmudflap -o $@ $<

test/pass%.x: test/pass%.c
	$(CC) -DSHOULD_PASS -fmudflap -o $@ $<


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
