/* Configuration for GNU C-compiler for hosting on Windows32.
   using GNU tools and the Windows32 API Library.
   Copyright (C) 1997, 1998, 1999, 2001, 2002 Free Software Foundation, Inc.

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

/* Even though we support "/", allow "\" since everybody tests both.  */
#define DIR_SEPARATOR '/'
#define DIR_SEPARATOR_2 '\\'

/* Mingw32 does not try to hide the underlying DOS-based file system
   like Cygwin does.  */
#define HAVE_DOS_BASED_FILE_SYSTEM

#define HOST_EXECUTABLE_SUFFIX ".exe"

#undef PATH_SEPARATOR
#define PATH_SEPARATOR ';'

#define HOST_BIT_BUCKET "nul"

#define HOST_WIDEST_INT long long
#define HOST_BITS_PER_WIDEST_INT HOST_BITS_PER_LONGLONG
#define HOST_WIDEST_INT_PRINT_DEC "%I64d"
#define HOST_WIDEST_INT_PRINT_UNSIGNED "%I64u"
#define HOST_WIDEST_INT_PRINT_HEX "0x%I64x"

/* This replaces the use of stat to determine if files are different
   in gcc.c (do_spec_1) handling of --save-temps switch.  */
   
extern int
w32_file_id_cmp PARAMS((const char *, const char *));
#define HOST_FILE_ID_CMP(SRC,DST) w32_file_id_cmp (SRC, DST)

