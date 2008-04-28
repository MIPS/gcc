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

#ifndef __LIBSTD_FCNTL_H
#define __LIBSTD_FCNTL_H

#if !defined __FCNTL_H__ && !defined __LIBSTD_CRT_BUILD
#  error "Never use <bits/libstd_fcntl.h> directly; include <fcntl.h> instead."
#endif

/* Remember to update values in src/MSCorlibWrapper.cs */

__BEGIN_EXTERN_C

enum {
    __LIBSTD_O_RDONLY =  0x0001,  /* open for reading only */
    __LIBSTD_O_WRONLY =  0x0002,  /* open for writing only */
    __LIBSTD_O_RDWR   =  0x0004,  /* open for reading and writing */
    __LIBSTD_O_APPEND =  0x0008,  /* writes done at eof */

    __LIBSTD_O_CREAT  =  0x0100,  /* create and open file */
    __LIBSTD_O_TRUNC  =  0x0200,  /* open and truncate */
    __LIBSTD_O_EXCL   =  0x0400,  /* exclusive open */

    /*  __O_TEXT files have cr-lf sequences translated to lf on read()'s,
     * and lf sequences translated to cr-lf on write()'s */
    __LIBSTD_O_TEXT   =  0x4000,  /* file mode is text (translated) */
    __LIBSTD_O_BINARY =  0x8000   /* file mode is binary (untranslated) */
};

__END_EXTERN_C

#endif /*__LIBSTD_FCNTL_H */
