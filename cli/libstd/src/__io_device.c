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

#include <__cdefs.h>

#include "__host.h"
#include <stdio.h>

#include "__io.h"

typedef struct __host_stream __host_stream;

struct __host_stream {
    signed char host_fd;
    char        in_use;
};

static __host_stream __host_stream_map[FOPEN_MAX] = {
    { 0, 1 }, /* stdin  */
    { 1, 1 }, /* stdout */
    { 2, 1 }  /* stderr */
};

LIBSTD_LPROTO_IMPL(int, __device_open, const char *path, int mode)
{
    int fileno = -1;
    int i;

    /* search the first free entry, skip 0, 1 and 2 */
    for (i=3; i<FOPEN_MAX && fileno==-1; ++i)
        if (!__host_stream_map[i].in_use) {
            fileno = i;
        }

    if (fileno != -1) {
        int dopen = LIBSTD_HNAME(open)((signed char *)path, mode);
        if (dopen != -1) {
            __host_stream_map[fileno].host_fd = dopen;
            __host_stream_map[fileno].in_use  = 1;
        }
        else
            fileno = -1;
    }
    return fileno;
}

LIBSTD_LPROTO_IMPL(int, __device_close, int fileno)
{
    if (   fileno >= 0 && fileno < FOPEN_MAX
        && __host_stream_map[fileno].in_use) {
        int close_result = LIBSTD_HNAME(close)(__host_stream_map[fileno].host_fd);
        if (close_result != -1)
            __host_stream_map[fileno].in_use = 0;
        return close_result;
    }
    return -1;
}

LIBSTD_LPROTO_IMPL(int, __device_read, int fileno, void *buf, int nbytes)
{
    if (   fileno >= 0 && fileno < FOPEN_MAX
        && __host_stream_map[fileno].in_use) {
        return LIBSTD_HNAME(read)(__host_stream_map[fileno].host_fd, buf, nbytes);
    }
    return 0;
}

LIBSTD_LPROTO_IMPL(int, __device_write, int fileno, const void *buf,  int nbytes)
{
    if (   fileno >= 0 && fileno < FOPEN_MAX
        && __host_stream_map[fileno].in_use) {
        return LIBSTD_HNAME(write)(__host_stream_map[fileno].host_fd, (void *)buf, nbytes);
    }
    return 0;
}

LIBSTD_LPROTO_IMPL(long, __device_lseek, int fileno, long int offset, int whence)
{
    if (   fileno >= 0 && fileno < FOPEN_MAX
        && __host_stream_map[fileno].in_use) {
        return LIBSTD_HNAME(lseek)(__host_stream_map[fileno].host_fd, offset, whence);
    }
    return 0;
}
