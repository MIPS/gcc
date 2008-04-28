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
#include <fcntl.h>

#include "__io.h"

LIBSTD_LPROTO_IMPL(FILE *, __io_fopen, FILE *fp, const char *name, const char *mode)
{
    if (fp) {
	/*
	 * "r"   open text file for reading
	 * "w"   truncate to zero or create file for writing
	 * "a"   open or create text file for writing at eof
	 * "rb"  open binary file for reading
	 * "wb"  truncate to zero or create binary file for writing
	 * "ab"  open or create binary file for writing at eof
	 * "r+"  open text file for reading and writing
	 * "w+"  truncate to zero or create text file for writing
	 * "a+"  open or create text file for writing at eof
	 * "r+b" or "rb+" open binary file for update
	 * "w+b" or "wb+" truncate to zero or create binary file for writing
	 * "a+b" or "ab+" open or create binary file for update, writing at eof
	 */
	if (mode && *mode) {
            int lowflags_;
            int binary_ = 0;
            int plus_ = 0;

            /* We could have mode + 1 set */
            if (mode[1]) {
                switch (mode[1]) {
                case '+': plus_   = 1; break;
                case 'b': binary_ = 1; break;
                default:
                    break;
                }
                if (mode[2]) {
                    switch (mode[2]) {
                    case '+': plus_   |= 1; break;
                    case 'b': binary_ |= 1; break;
                    default:
                        break;
                    }
                }
            }

            switch (*mode) {
	    case 'r':
                lowflags_ = plus_ ? O_RDWR : O_RDONLY;
                _FILE_SET_r(fp);
                break;
	    case 'w':
                lowflags_ = (plus_ ? O_RDWR : O_WRONLY ) | O_CREAT | O_TRUNC;
                _FILE_SET_w(fp);
                break;
	    case 'a':
                lowflags_ = (plus_ ? O_RDWR : O_WRONLY ) | O_CREAT | O_APPEND;
                _FILE_SET_rw(fp);
                break;
	    default:
                break;
	    }
            if (plus_)
                _FILE_SET_rw(fp);
            if (binary_) {
                _FILE_SET_bin(fp);
                lowflags_ |= O_BINARY;
            }
	    fp->fd = LIBSTD_LNAME(__device_open)(name, lowflags_);

	    if (fp->fd < 0 ) {
		LIBSTD_LNAME(__io_ftable_release_file)(fp);
		fp = NULL;
	    }
	} else {
	    /* No mode, we consider this a failure.*/
	    LIBSTD_LNAME(__io_ftable_release_file)(fp);
	    fp = NULL;
	}
    }
    return fp;
}
