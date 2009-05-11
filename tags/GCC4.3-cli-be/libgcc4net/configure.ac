# Process this file with autoconf to produce a configure script, like so:
# 
# aclocal -I .. -I ../config && autoconf && automake

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


AC_PREREQ(2.59)
AC_INIT(libgcc4net, 0.1)
AC_CONFIG_SRCDIR(gcc4net.cs)

AM_INIT_AUTOMAKE([1.9.5 foreign])

AM_MAINTAINER_MODE

AC_ARG_WITH(mcs,
	    AS_HELP_STRING([--with-mcs=mcs_binary_path], [use this mcs]),
	    MCS=$withval)
AC_CHECK_PROG(MCS,mcs,mcs,mcs-not-found)

toollibdir='$(libdir)'
toollib2dir='$(toollibdir)/gcc/$(target_alias)/$(gcc_version)'
AC_SUBST(toollibdir)
AC_SUBST(toollib2dir)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
