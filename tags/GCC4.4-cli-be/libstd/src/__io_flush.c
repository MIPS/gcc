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

LIBSTD_LPROTO_IMPL(int, __io_flush, FILE *fp)
{
    if (fp && fp->fd != -1) {
	int write_result = 0;
	size_t tobewritten = fp->pos - fp->base;
	if (tobewritten>0 && _FILE_GET_w(fp))
            write_result = LIBSTD_LNAME(__device_write)(fp->fd,
                                                       (const void *)fp->base,
                                                       (int)tobewritten);
	if (write_result < 0) {
            _FILE_SET_error(fp);
            return EOF;
	} else {
	    fp->pos  = fp->base;
	    fp->stop = NULL;
	    if (_FILE_GET_rw(fp)) {
                _FILE_UNSET_r(fp);
                _FILE_UNSET_w(fp);
            }
	    return 0;
	}
    }
    return EOF;
}
