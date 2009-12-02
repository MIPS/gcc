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

#if 0

#include <signal.h>
#include <errno.h>

#define __SIG_MAX       16

static __sighandler_t __signal__handler[__SIG_MAX] = {
    SIG_IGN, /* --IGNORED-- */
    SIG_DFL, /* SIGABRT */
    SIG_DFL, /* SIGFPE */
    SIG_DFL, /* SIGILL */
    SIG_DFL, /* SIGINT */
    SIG_DFL, /* SIGSEGV */
    SIG_DFL  /* SIGTERM */
};

LIBSTD_LPROTO_IMPL(__sighandler_t, signal, int sig, __sighandler_t func)
{
    __sighandler_t __handler;

    if (sig <= 0 || sig >= __SIG_MAX)
        return SIG_ERR;

    __handler = __signal__handler[sig];
    __signal__handler[sig] = func;
    return __handler;
}

LIBSTD_LPROTO_IMPL(int, raise, int sig)
{
    __sighandler_t __handler;

    if (sig <= 0 || sig >= __SIG_MAX)
        return -1;

    __handler = __signal__handler[sig];

    if ( __handler != SIG_IGN)
        (*__handler)(sig);

    return 0;
}

#endif /* 0 */
