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

#include <fenv.h>


LIBSTD_LPROTO_IMPL(void, fegetexceptflag, fexcept_t *flagp, int excepts)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(void, fesetexceptflag, const fexcept_t *flagp, int excepts)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(void, fegetenv, fenv_t *envp)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(void, fesetenv, const fenv_t *envp)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(int, feholdexcept, fenv_t *envp)
{
    return 0;
}

LIBSTD_LPROTO_IMPL(void, feupdateenv, const fenv_t *envp)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(int, fetestexcept, int excepts)
{
    /* EMPTY */
    return 0;
}

LIBSTD_LPROTO_IMPL(void, feraiseexcept, int excepts)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(void, feclearexcept, int excepts)
{
    /* EMPTY */
}

LIBSTD_LPROTO_IMPL(int, fegetround, void)
{
    return FE_TONEAREST;
}

LIBSTD_LPROTO_IMPL(int, fesetround, int eound)
{
    return 1;
}
