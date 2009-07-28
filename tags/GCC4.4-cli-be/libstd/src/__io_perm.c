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

/* If we are compiling this, we are sure this is to build C-runtime*/
#define __LIBSTD_CRT_BUILD

#include <stdio.h>

#include "__io.h"

LIBSTD_LPROTO_IMPL(int, __io_perm_readable, FILE *fp)
{
    if (fp->fd != -1 && (_FILE_GET_r(fp) || _FILE_GET_rw(fp))) {

        if (_FILE_GET_rw(fp))
            _FILE_SET_r(fp);

        if (!fp->base && _FILE_GET_buff(fp) != _IONBF) {
            if (!LIBSTD_LNAME(setvbuf)(fp, NULL, _FILE_GET_buff(fp) , BUFSIZ))
                return 1;
        }
        else
            return 1;
    }
    return 0;
}

LIBSTD_LPROTO_IMPL(int, __io_perm_writable, FILE *fp)
{
    if (fp->fd != -1 && (_FILE_GET_w(fp) || _FILE_GET_rw(fp))) {

        if (_FILE_GET_rw(fp))
            _FILE_SET_w(fp);

        if (!fp->base && _FILE_GET_buff(fp) != _IONBF) {
            if (!LIBSTD_LNAME(setvbuf)(fp, NULL, _FILE_GET_buff(fp) , BUFSIZ))
                return 1;
        } else
            return 1;

    }
    return 0;
}

