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
#include <stdlib.h>

#include "__io.h"

LIBSTD_SPROTO(void, __do__atexit, void);

static int __atexit_records = 0;
static __atexit_fun __atexit_fun_table[__ATEXIT_MAX];

/* Registered functions called when exit is executed. */
LIBSTD_SPROTO_IMPL(void, __do__atexit, void)
{
    int i = __atexit_records-1;
    for (; i>=0; --i)
        (*(__atexit_fun_table[i]))();
}

LIBSTD_LPROTO_IMPL(int, atexit, __atexit_fun func)
{
    if (   func /* refuse to register NULL function pointers */
        && (__atexit_records - 1 < __ATEXIT_MAX))
        {
            __atexit_fun_table[__atexit_records++] = func;
            return 0;
        }
    return 1;
}

LIBSTD_LPROTO_IMPL(void, exit, int status)
{
    /* all functions registered by the atexit function are called.
     * in the reverse order of their registration
     */
    LIBSTD_SNAME(__do__atexit)();

    /* all open streams with unwritten buffered data are flushed,
     * all open streams are closed,
     * all files created by the tmpfile function are removed.
     */
    LIBSTD_LNAME(__io_ftable_close_all());

    /* Giving control back to host */
    LIBSTD_HNAME(exit)(status);
}
