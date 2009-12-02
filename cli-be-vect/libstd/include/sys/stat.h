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

#ifndef __SYS_STAT_H__
#define __SYS_STAT_H__

#include <__cdefs.h>

/* Included for off_t */
#include <sys/types.h>

__BEGIN_EXTERN_C

struct stat {
    off_t st_size;  /* total size, in bytes */
};

LIBSTD_LPROTO(int, stat, const char *, struct stat *);
LIBSTD_LPROTO(int, fstat, int, struct stat *);

__END_EXTERN_C

#endif /*__SYS_STAT_H__*/
