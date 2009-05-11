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

#include <stdlib.h>

static unsigned int __seed = 1;

LIBSTD_LPROTO_IMPL(int, rand, void)
{
    __seed *= 1103515245;
    __seed += 12345;
    return __seed / (2*(RAND_MAX + 1)) % (RAND_MAX + 1);
}

LIBSTD_LPROTO_IMPL(void, srand, unsigned int seed)
{
    __seed = seed;
}
