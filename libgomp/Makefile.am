## Makefile for gomp.                                            -*-Automake-*-
##
## $Id: Makefile.am,v 1.1.1.1 2004/10/21 20:51:50 biagio Exp $
## 
## Copyright (c) 2004
##			see AUTHORS list
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#auxdir          = @ac_aux_dir@

#AUX_DIST        = \
#                                $(auxdir)/install-sh \
#                                $(auxdir)/missing \
#                                $(auxdir)/mkinstalldirs \
#                                $(auxdir)/config.guess \
#                                $(auxdir)/config.sub \
#                                $(auxdir)/ltmain.sh

EXTRA_DIST = README

lib_LTLIBRARIES = \
	libgomp.la

libgomp_la_LDFLAGS = \
	-module \
	-version-info `grep -v '^\#' $(srcdir)/libtool-version` 

libgomp_la_SOURCES = \
	lib/ibgomp.h \
	lib/gomp.h \
	lib/gomp.c \
	lib/destroy_lock.c \
	lib/destroy_nest_lock.c \
	lib/get_dynamic.c \
	lib/get_max_threads.c \
	lib/get_nested.c \
	lib/get_num_procs.c \
	lib/get_num_threads.c \
	lib/get_thread_num.c \
	lib/get_wtick.c \
	lib/get_wtime.c \
	lib/in_parallel.c \
	lib/init_lock.c \
	lib/init_nest_lock.c \
	lib/set_dynamic.c \
	lib/set_lock.c \
	lib/set_nest_lock.c \
	lib/set_nested.c \
	lib/set_num_threads.c \
	lib/test_lock.c \
	lib/test_nest_lock.c \
	lib/unset_lock.c \
	lib/unset_nest_lock.c

include_HEADERS = lib/libgomp.h 

MAINTAINERCLEANFILES    =   \
        $(AUX_DIST)         \
        Makefile.in         \
        aclocal.m4          \
        config-h.in         \
        configure           \
        stamp-h.in

maintainer-clean-local:
	rm -f gomp*.tar.gz

dist-hook:
	(cd $(distdir) && mkdir -p $(auxdir))
	for file in $(AUX_DIST); do \
		cp $$file $(distdir)/$$file; \
	done
