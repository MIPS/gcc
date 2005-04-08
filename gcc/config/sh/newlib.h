/* Definitions of target machine for gcc for Super-H using sh-superh-elf.
   Copyright (C) 2001 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* This header file is used when the vendor name is set to 'superh'.
   It configures the compiler for SH4 only and switches the default
   endianess to little (although big endian is still available).
   It also configures the spec file to use the 7750 board configuration
   but in such a way that it can be overriden by a boardspecs file
   (using the -specs= option). This file is expected to disable the
   defaults and provide options --defsym _start and --defsym _stack
   which are required by the SuperH configuration of GNU ld.

   This file sets a few spec strings when using newlib and libgloss */

#undef LIB_SPEC
#define LIB_SPEC "-lc -lgloss"
