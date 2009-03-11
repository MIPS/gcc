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
#include <string.h>

#include "__io.h"

static FILE __ftable[FOPEN_MAX] = {
    /* fd, flags, base, end, pos, stop, tmpnam, unget_val */
    /* stdin  */ {  0, __iob_mask_r | _IOLBF | __iob_mask_tty },
    /* stdout */ {  1, __iob_mask_w | _IONBF | __iob_mask_tty },
    /* stderr */ {  2, __iob_mask_w | _IONBF | __iob_mask_tty },
    /* Free   */ { -1 },
    /* Free   */ { -1 },
    /* Free   */ { -1 },
    /* Free   */ { -1 },
    /* Free   */ { -1 }
};

static const FILE pattern_new_     = { -1, _IOFBF };

LIBSTD_LPROTO_IMPL(FILE *, __io_ftable_new_file, void)
{
    int i;
    for (i=0; i< FOPEN_MAX; ++i)
        if (__ftable[i].fd == -1) {
            __ftable[i] = pattern_new_;
            return &__ftable[i];
        }
    return NULL;
}

static const FILE pattern_release_ = { -1, _IOFBF };

LIBSTD_LPROTO_IMPL(void, __io_ftable_release_file, FILE *fp)
{
    if (fp) {
        if ((fp->fd >=0) && (fp->fd < FOPEN_MAX) && (fp == &__ftable[fp->fd])) {
            __ftable[fp->fd] = pattern_release_;
        } else {
            int i;
            for (i=0; i< FOPEN_MAX; ++i)
                if (fp == &__ftable[i]) {
                    __ftable[i] = pattern_release_;
                    return;
                }
        }
    }
}

LIBSTD_LPROTO_IMPL(FILE *, __io_ftable_get_entry, int fileno)
{
    if (__ftable[fileno].fd != -1)
        return &__ftable[fileno];
    else
        return NULL;
}

LIBSTD_LPROTO_IMPL(void, __io_ftable_close_all, void)
{
    int i;
    for (i=0; i< FOPEN_MAX; ++i) {
        switch (__ftable[i].fd) {
        case -1:
            /* Descriptor not used */
            break;
        case 0:
            break;
        case 1:
        case 2:
            LIBSTD_LNAME(__io_flush)(&__ftable[i]);
            break;
        default:
            LIBSTD_LNAME(fclose)(&__ftable[i]);
            break;
        }
    }
}

LIBSTD_LPROTO_IMPL(int, __io_ftable_flush_all, void)
{
    int i;
    int result = 0;
    for (i=0; i< FOPEN_MAX; ++i)
        if (__ftable[i].fd != -1)
            result |= LIBSTD_LNAME(__io_flush)(&__ftable[i]);
    return result;
}
