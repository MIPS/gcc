/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef __LIBSTD_STDIO_H
#define __LIBSTD_STDIO_H

#if !defined(__STDIO_H__) && ! defined(__UNISTD_H) && !defined(__LIBSTD_CRT_BUILD)
#  error "Never use <bits/libstd_stdio.h> directly; include <stdio.h> or <unistd.h> instead."
#endif

/* Remember to update values in src/MSCorlibWrapper.cs */

__BEGIN_EXTERN_C

enum {
    __LIBSTD_FOPEN_MAX = 8
};

enum {
    __LIBSTD_FILENAME_MAX = 256
};

enum {
    __LIBSTD_BUFSIZ = 2048
};

enum {
    __LIBSTD_IOFBF = 0,
    __LIBSTD_IOLBF = 1,
    __LIBSTD_IONBF = 2
};

enum {
    __LIBSTD_SEEK_SET = 0,
    __LIBSTD_SEEK_CUR = 1,
    __LIBSTD_SEEK_END = 2
};

enum {
    __LIBSTD_L_tmpnam = 32
};
enum {
    __LIBSTD_TMP_MAX = 17576   /* Vary 3 lower cases letters = 26*26*26 */
};

enum {
    __LIBSTD_STDIN_FILENO  = 0,
    __LIBSTD_STDOUT_FILENO = 1,
    __LIBSTD_STDERR_FILENO = 2
};

__END_EXTERN_C

#endif /*__LIBSTD_STDIO_H */
