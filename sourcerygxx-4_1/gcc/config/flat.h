/* Defines to be used for targets that support flat executables.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

/* This macro applies on top of OBJECT_FORMAT_ELF and indicates that
   we want to support both flat and ELF output.  */
#define OBJECT_FORMAT_FLAT

/* Disable the use of the comdat groups on targets that support flat
   executables.  elf2flt uses ld -Ur with a scripts that combines all
   text sections into .text, all data sections in .data, etc.  This works
   for linkonce sections, but does not work for comdat groups, which are
   only subject to script placement in non-relocatable links.  */
#define TARGET_USE_COMDAT_GROUPS false
