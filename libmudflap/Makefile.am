## Makefile for the toplevel directory of the mudflap library.
##
## Copyright (C) 2002
## Free Software Foundation, Inc.
##

AUTOMAKE_OPTIONS = 1.3 cygnus
MAINT_CHARSET = latin1

TESTS_ENVIRONMENT = LD_LIBRARY_PATH='.libs:../../gcc:../libstdc++-v3/src/.libs' MUDFLAP_OPTIONS='-viol-abort -no-heur-proc-map'

TESTS = test/fail1.x test/fail2.x test/fail3.x test/fail4.x test/fail5.x \
 test/fail6.x test/fail7.x test/fail8.x test/fail9.x test/fail10.x \
 test/fail11.x test/fail12.x test/fail13.x test/fail14.x test/fail15.x \
 test/fail16.x test/fail17.x test/fail18.x test/fail19.x test/fail20.x \
 test/fail21.x test/fail22.x \
 test/pass1.x test/pass2.x test/pass3.x test/pass4.x test/pass5.x \
 test/pass6.x test/pass7.x test/pass8.x test/pass9.x test/pass10.x \
 test/pass11.x test/pass12.x test/pass13.x test/pass14.x test/pass15.x \
 test/pass16.x test/pass17.x test/pass18.x test/pass19.x test/pass20.x \
 test/pass21.x test/pass22.x test/pass23.x test/pass24.x test/pass25.x \
 test/pass26.x test/pass27.xx test/pass28.xx test/pass29.x \
 test/pass-stratcliff.x

test/%.c: test/%-frag.c test/mf-driver.c
	@mkdir -p test
	@echo '#include "'$(srcdir)/test/mf-driver.c'"' > $@
	@echo 'void test() {' >> $@
	@echo '#include "'$<'"' >> $@
	@echo '}' >> $@
	@echo created $@

test/%.cxx: test/%-frag.cxx test/mf-driver.c
	@mkdir -p test
	@echo '#include "'$(srcdir)/test/mf-driver.c'"' > $@
	@echo 'void test() {' >> $@
	@echo '#include "'$<'"' >> $@
	@echo '}' >> $@
	@echo created $@

clean-local:
	rm -f $(TESTS)

# Various possibilities
#TESTFLAGS=-rdynamic
TESTFLAGS=-static -O2
#TESTFLAGS=-O3

test/fail%.x: test/fail%.c
	$(CC) -DSHOULD_FAIL -fmudflap $(TESTFLAGS) -g -o $@ $<

test/pass%.x: test/pass%.c
	$(CC) -DSHOULD_PASS -fmudflap $(TESTFLAGS) -g -o $@ $<

test/fail%.xx: test/fail%.cxx
	$(CXX) -DSHOULD_FAIL -fmudflap $(TESTFLAGS) -g -o $@ $<

test/pass%.xx: test/pass%.cxx
	$(CXX) -DSHOULD_PASS -fmudflap $(TESTFLAGS) -g -o $@ $<

$(TESTS): clean-tests $(lib_LTLIBRARIES)

clean-tests:
	rm -f $(TESTS)

# SUBDIRS = 
# PWD = $${PWDCMD-pwd}

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

