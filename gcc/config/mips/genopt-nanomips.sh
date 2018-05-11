#!/bin/sh
# Generate mips-tables.opt from the list of CPUs in mips-cpus.def.
# Copyright (C) 2011-2016 Free Software Foundation, Inc.
#
# This file is part of GCC.
#
# GCC is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GCC is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GCC; see the file COPYING3.  If not see
# <http://www.gnu.org/licenses/>.

cat <<EOF
; -*- buffer-read-only: t -*-
; Generated automatically by genopt-nanomips.sh from nanomips-cpus.def.

; Copyright (C) 2017 Free Software Foundation, Inc.
;
; This file is part of GCC.
;
; GCC is free software; you can redistribute it and/or modify it under
; the terms of the GNU General Public License as published by the Free
; Software Foundation; either version 3, or (at your option) any later
; version.
;
; GCC is distributed in the hope that it will be useful, but WITHOUT ANY
; WARRANTY; without even the implied warranty of MERCHANTABILITY or
; FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
; for more details.
;
; You should have received a copy of the GNU General Public License
; along with GCC; see the file COPYING3.  If not see
; <http://www.gnu.org/licenses/>.

Enum
Name(mips_arch_opt_value) Type(int)
Known MIPS CPUs (for use with the -march= and -mtune= options):

Enum
Name(mips_mips_opt_value) Type(int)

EnumValue
Enum(mips_arch_opt_value) String(native) Value(MIPS_ARCH_OPTION_NATIVE) DriverOnly

EOF

awk -F'[(, 	]+' '
BEGIN {
    value = 0
}

# Write an entry for a single string accepted as a -march= argument.

function write_one_arch_value(name, value)
{
    print "EnumValue"
    print "Enum(mips_arch_opt_value) String(" name ") Value(" value ")"
    print ""
}

/^MIPS_CPU/ {
    name = $2
    gsub("\"", "", name)
    write_one_arch_value(name, value)
    value++
}' $1/nanomips-cpus.def
