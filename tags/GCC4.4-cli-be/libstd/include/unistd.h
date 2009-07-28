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

#ifndef __UNISTD_H
#define __UNISTD_H

#include <__cdefs.h>

/* Included for NULL, size_t */
#include <stddef.h>

/* Included for off_t, ssize_t */
#include <sys/types.h>

#include <bits/libstd_stdio.h>

__BEGIN_EXTERN_C

LIBSTD_LPROTO(int, close, int);
LIBSTD_LPROTO(ssize_t, read, int, void *, size_t);
LIBSTD_LPROTO(ssize_t, write, int, const void *, size_t);
LIBSTD_LPROTO(off_t, lseek, int, off_t, int);

#define STDIN_FILENO  __LIBSTD_STDIN_FILENO
#define STDOUT_FILENO __LIBSTD_STDOUT_FILENO
#define STDERR_FILENO __LIBSTD_STDERR_FILENO

LIBSTD_LPROTO(int, isatty, int);

__END_EXTERN_C

#endif /*__UNISTD_H__*/
