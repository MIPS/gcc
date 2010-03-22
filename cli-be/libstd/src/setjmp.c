/*

   Copyright (C) 2010 Free Software Foundation, Inc.
   Contributed by INRIA

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

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

LIBSTD_LPROTO_IMPL(int, setjmp, jmp_buf buf)
{
    return 0;
}

LIBSTD_LPROTO_IMPL(void, longjmp, jmp_buf buf, int val)
{
    LIBSTD_LNAME(fprintf)(stderr, "** longjmp not implemented **\n");
    LIBSTD_LNAME(exit)(EXIT_FAILURE);
}
