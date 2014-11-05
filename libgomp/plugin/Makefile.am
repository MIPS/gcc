# Plugins for offload execution.
#
# Copyright (C) 2014 Free Software Foundation, Inc.
#
# Contributed by Mentor Embedded.
#
# This file is part of the GNU OpenMP Library (libgomp).
#
# Libgomp is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# Libgomp is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# Under Section 7 of GPL version 3, you are granted additional
# permissions described in the GCC Runtime Library Exception, version
# 3.1, as published by the Free Software Foundation.
#
# You should have received a copy of the GNU General Public License and
# a copy of the GCC Runtime Library Exception along with this program;
# see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
# <http://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = -I ../.. -I ../../config

config_path = @config_path@
search_path = .. $(addprefix $(top_srcdir)/../config/, $(config_path)) \
	      $(top_srcdir) $(top_srcdir)/../../include $(top_srcdir)/..

AM_CPPFLAGS = $(addprefix -I, $(search_path))
AM_CFLAGS = $(XCFLAGS)
AM_LDFLAGS = $(XLDFLAGS) $(SECTION_LDFLAGS) $(OPT_LDFLAGS)

toolexeclib_LTLIBRARIES =

if PLUGIN_NVPTX
# Nvidia PTX OpenACC plugin.
libgomp_plugin_nvptx_version_info = -version-info $(libtool_VERSION)
toolexeclib_LTLIBRARIES += libgomp-plugin-nvptx.la
libgomp_plugin_nvptx_la_SOURCES = plugin-nvptx.c
libgomp_plugin_nvptx_la_CPPFLAGS = $(AM_CPPFLAGS) $(PLUGIN_NVPTX_CPPFLAGS)
libgomp_plugin_nvptx_la_LDFLAGS = $(libgomp_plugin_nvptx_version_info) \
	$(lt_host_flags)
libgomp_plugin_nvptx_la_LDFLAGS += $(PLUGIN_NVPTX_LDFLAGS)
libgomp_plugin_nvptx_la_LIBADD = $(PLUGIN_NVPTX_LIBS)
libgomp_plugin_nvptx_la_LIBTOOLFLAGS = --tag=disable-static
endif

libgomp_plugin_host_nonshm_version_info = -version-info $(libtool_VERSION)
toolexeclib_LTLIBRARIES += libgomp-plugin-host_nonshm.la
libgomp_plugin_host_nonshm_la_SOURCES = plugin-host.c
libgomp_plugin_host_nonshm_la_CPPFLAGS = $(AM_CPPFLAGS) -DHOST_NONSHM_PLUGIN
libgomp_plugin_host_nonshm_la_LDFLAGS = \
	$(libgomp_plugin_host_nonshm_version_info) $(lt_host_flags)
libgomp_plugin_host_nonshm_la_LIBTOOLFLAGS = --tag=disable-static

LTLDFLAGS = $(shell $(SHELL) $(top_srcdir)/../../libtool-ldflags $(LDFLAGS))

LINK = $(LIBTOOL) --tag CC $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link \
	$(CCLD) $(AM_CFLAGS) $(CFLAGS) $(AM_LDFLAGS) $(LTLDFLAGS) -o $@
