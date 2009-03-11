
#   Copyright (C) 2007 Free Software Foundation, Inc.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#
# Authors:
#    Andrea Ornstein
#    Erven Rohou
#    Roberto Costa
#
# Contact information at STMicroelectronics:
# Andrea C. Ornstein    <andrea.ornstein@st.com>
# Erven Rohou           <erven.rohou@st.com>

ACLOCAL_AMFLAGS = -I .. -I ../config

# May be used by various substitution variables.
gcc_version := $(shell cat $(top_srcdir)/../gcc/BASE-VER)

EXEEXT=
toollib_PROGRAMS = gcc4net.dll
toollib2_PROGRAMS = gcc4net.dll

gcc4net_dll_SOURCES=gcc4net.cs

gcc4net.dll: gcc4net.cs
	$(MCS) $< -unsafe -target:library -out:$@

# No install-html or install-pdf support in automake yet
.PHONY: install-html install-pdf
install-html:

install-pdf:

install-toollibPROGRAMS: $(toollib_PROGRAMS)
	test -z "$(toollibdir)" || $(mkdir_p) "$(DESTDIR)$(toollibdir)"
	@list='$(toollib_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_PROGRAM_ENV) $(toollibPROGRAMS_INSTALL) '$$p' '$(DESTDIR)$(toollibdir)/$$p'"; \
	    $(INSTALL_PROGRAM_ENV) $(toollibPROGRAMS_INSTALL) "$$p" "$(DESTDIR)$(toollibdir)/$$p" || exit 1; \
	  else :; fi; \
	done

uninstall-toollibPROGRAMS:
	@list='$(toollib_PROGRAMS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(toollibdir)/$$p'"; \
	  rm -f "$(DESTDIR)$(toollibdir)/$$p"; \
	done

install-toollib2PROGRAMS: $(toollib2_PROGRAMS)
	test -z "$(toollib2dir)" || $(mkdir_p) "$(DESTDIR)$(toollib2dir)"
	@list='$(toollib2_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	   echo " $(INSTALL_PROGRAM_ENV) $(toollib2PROGRAMS_INSTALL) '$$p' '$(DESTDIR)$(toollib2dir)/$$p'"; \
	   $(INSTALL_PROGRAM_ENV) $(toollib2PROGRAMS_INSTALL) "$$p" "$(DESTDIR)$(toollib2dir)/$$p" || exit 1; \
	  else :; fi; \
	done

uninstall-toollib2PROGRAMS:
	@list='$(toollib2_PROGRAMS)'; for p in $$list; do \
	  echo " rm -f '$(DESTDIR)$(toollib2dir)/$$p'"; \
	  rm -f "$(DESTDIR)$(toollib2dir)/$$p"; \
	done
