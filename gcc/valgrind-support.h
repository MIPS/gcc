/* Support for --enable-checking=valgrind.
   Copyright (C) 2006
   Free Software Foundation, Inc.

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

#ifndef GCC_VALGRIND_SUPPORT_H
#define GCC_VALGRIND_SUPPORT_H

#ifdef ENABLE_VALGRIND_CHECKING
# ifdef HAVE_VALGRIND_MEMCHECK_H
#  include <valgrind/memcheck.h>
# elif defined HAVE_MEMCHECK_H
#  include <memcheck.h>
# else
#  include <valgrind.h>
# endif
#else
/* Avoid #ifdef:s when we can help it.  */
#define VALGRIND_DISCARD(x)
#define VALGRIND_MALLOCLIKE_BLOCK(x, y, z, t)
#define VALGRIND_FREELIKE_BLOCK(x, y)
#define VALGRIND_MAKE_NOACCESS(x, y)
#endif

#endif
