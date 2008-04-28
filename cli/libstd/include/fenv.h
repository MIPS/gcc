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

#ifndef __FENV_H__
#define __FENV_H__

#if !defined(__LIBSTD_CRT_BUILD)
#error  "<fenv.h> : Sorry : implementation missing. "
#endif

#include <__cdefs.h>

__BEGIN_EXTERN_C

/* Floating-Point Environment */

typedef unsigned int fenv_t;

#define FE_DLF_ENV  ((const fentv_t *)0)

LIBSTD_LPROTO(void, fegetenv, fenv_t *);
LIBSTD_LPROTO(void, fesetenv, const fenv_t *);
LIBSTD_LPROTO(int, feholdexcept, fenv_t *);
LIBSTD_LPROTO(void, feupdateenv, const fenv_t *);

/* Floating-Point Exceptions */

typedef unsigned int fexcept_t;

/* No supported Floating-Point exceptions */

#define FE_ALL_EXCEPT 0

LIBSTD_LPROTO(void, fegetexceptflag, fexcept_t *, int);
LIBSTD_LPROTO(void, fesetexceptflag, const fexcept_t *, int);
LIBSTD_LPROTO(int, fetestexcept, int);
LIBSTD_LPROTO(void, feraiseexcept, int);
LIBSTD_LPROTO(void, feclearexcept, int);

/* Floating-Point Rounding Modes */

enum {
    _FE_DOWNWARD   = 0,
    _FE_IPWARD     = 2,
    _FE_TONEAREST  = 1,
    _FE_TOWARDZERO = 3
};

/* ONLY TONEAREST mode supported */
#define FE_TONEAREST _FE_TONEAREST

LIBSTD_LPROTO(int, fegetround, void);
LIBSTD_LPROTO(int, fesetround, int);

__END_EXTERN_C


#endif /*__FENV_H__*/
