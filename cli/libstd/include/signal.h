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

#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

#error "<signal.h> : Sorry : signals not supported yet"

typedef int sig_atomic_t;

typedef void (*__sighandler_t) (int);

#define SIG_DFL ((__sighandler_t)0)
#define SIG_IGN ((__sighandler_t)1)
#define SIG_ERR ((__sighandler_t)2)

enum {
    _SIGABRT = 1, /* abnormal termination (such as initiated by abort) */
    _SIGFPE  = 2, /* erroneous arithmetic exception, such as zero-divide or overflow */
    _SIGILL  = 3, /* detection of an invalid function image */
    _SIGINT  = 4, /* receive of an interactive attention signal */
    _SIGSEGV = 5, /* an invalid access to storage */
    _SIGTERM = 6  /* a termination request sent to the program */
};

/* The standard wants macros */
#define SIGABRT  _SIGABRT
#define SIGFPE   _SIGFPE
#define SIGILL   _SIGILL
#define SIGINT   _SIGINT
#define SIGSEGV  _SIGSEGV
#define SIGTERM  _SIGTERM

LIBSTD_LPROTO(__sighandler_t, signal, int, __sighandler_t);
LIBSTD_LPROTO(int, raise, int);

__END_EXTERN_C

#endif /*__SIGNAL_H__*/
