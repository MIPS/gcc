/* proj.h
   Copyright (C) 1995, 1996, 1999 Free Software Foundation, Inc.
   Contributed by James Craig Burley.

This file is part of GNU Fortran.

GNU Fortran is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Fortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Fortran; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef _H_f_proj
#define _H_f_proj

#ifdef USE_HCONFIG
#include "hconfig.j"
#else
#include "config.j"
#endif
#include "system.j"

#if ! defined (__STDC__)
#error "You have to use an ANSI C compiler to build g77"
#endif

#ifndef BUILT_WITH_270
#if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
#define BUILT_WITH_270 1
#else
#define BUILT_WITH_270 0
#endif
#endif	/* !defined (BUILT_WITH_270) */

/* Include files everyone gets.  <assert.h> is needed for assert().
   <stddef.h> is needed for offsetof, but technically also NULL,
   size_t, ptrdiff_t, and so on.  */

#include "assert.j"

#if HAVE_STDDEF_H
#include <stddef.h>
#endif

#define ARRAY_SIZE(a) (sizeof((a))/sizeof((a)[0]))

#ifndef UNUSED	/* Compile with -DUNUSED= if cc doesn't support this. */
#if BUILT_WITH_270
#define UNUSED __attribute__ ((unused))
#else	/* ! BUILT_WITH_270 */
#define UNUSED
#endif	/* ! BUILT_WITH_270 */
#endif  /* ! defined (UNUSED) */

#ifndef dmpout
#define dmpout stderr
#endif

#endif
