#!/bin/sh
# Copyright (C) 2006 Free Software Foundation, Inc.
# This file is part of GCC.

# GCC is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# GCC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING.  If not, write to
# the Free Software Foundation, 51 Franklin Street, Fifth Floor,
# Boston MA 02110-1301, USA.

# Go through the multilib matches in $matches.  For each match of the
# form mcpu?<core>=<option>, check whether the sysroot in $sysroot
# has a subdirectory called m<core>.  Use that subdirectory for
# <option> if so.
set -e
spec=
for match
do
  case $match in
    mcpu[?]*=*)
      option=`echo "$match" | sed -e 's/.*=//' -e 's/?/=/g'`
      suffix=`echo "$match" | sed -e 's/=.*//' -e 's/cpu?//g'`
      if test -d "$sysroot/$suffix"; then
        test -z "$spec" || spec="$spec;"
        spec="$spec$option:/$suffix"
      fi
      ;;
  esac
done
if test -n "$spec"; then
  echo "#undef SYSROOT_SUFFIX_SPEC"
  echo "#define SYSROOT_SUFFIX_SPEC \"%{$spec}\""
fi
