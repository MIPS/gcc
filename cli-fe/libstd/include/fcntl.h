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

#ifndef __FCNTL_H__
#define __FCNTL_H__

#include <__cdefs.h>

/* Included for NULL */
#include <stddef.h>

#include <bits/libstd_fcntl.h>

__BEGIN_EXTERN_C

#define O_RDONLY  __LIBSTD_O_RDONLY
#define O_WRONLY  __LIBSTD_O_WRONLY
#define O_RDWR    __LIBSTD_O_RDWR
#define O_APPEND  __LIBSTD_O_APPEND

#define O_CREAT   __LIBSTD_O_CREAT
#define O_TRUNC   __LIBSTD_O_TRUNC
#define O_EXCL    __LIBSTD_O_EXCL

#define O_TEXT    __LIBSTD_O_TEXT
#define O_BINARY  __LIBSTD_O_BINARY

LIBSTD_LPROTO(int, open, const char *, int, ...);
LIBSTD_LPROTO(int, creat, const char *, int);

__END_EXTERN_C

#endif /*__FCNTL_H__*/
