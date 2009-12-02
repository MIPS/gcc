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

#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <__cdefs.h>

__BEGIN_EXTERN_C

#if defined __IS_ISO_C99

#error  "<complex.h> : Sorry : complex types not supported yet. "

#define complex _Complex
#define imaginary _Imaginary

#define _Complex_I ...
#define _Imaginary_I ...
#define I  _Imaginary_I

LIBSTD_LPROTO(double complex, cacos, double complex);
LIBSTD_LPROTO(float complex, cacosf, float complex);
LIBSTD_LPROTO(long double complex, cacosl, long double complex);

LIBSTD_LPROTO(double complex, casin, double complex);
LIBSTD_LPROTO(float complex, casinf, float complex);
LIBSTD_LPROTO(long double complex, casinl, long double complex);

LIBSTD_LPROTO(double complex, catan, double complex);
LIBSTD_LPROTO(float complex, catanf, float complex);
LIBSTD_LPROTO(long double complex, catanl, long double complex);

LIBSTD_LPROTO(double complex, ccos, double complex);
LIBSTD_LPROTO(float complex, ccosf, float complex);
LIBSTD_LPROTO(long double complex, ccosl, long double complex);

LIBSTD_LPROTO(double complex, csin, double complex);
LIBSTD_LPROTO(float complex, csinf, float complex);
LIBSTD_LPROTO(long double complex, csinl, long double complex);

LIBSTD_LPROTO(double complex, ctan, double complex);
LIBSTD_LPROTO(float complex, ctanf, float complex);
LIBSTD_LPROTO(long double complex, ctanl, long double complex);

LIBSTD_LPROTO(double complex, cacosh, double complex);
LIBSTD_LPROTO(float complex, cacoshf, float complex);
LIBSTD_LPROTO(long double complex, cacoshl, long double complex);

LIBSTD_LPROTO(double complex, casinh, double complex);
LIBSTD_LPROTO(float complex, casinhf, float complex);
LIBSTD_LPROTO(long double complex, casinhl, long double complex);

LIBSTD_LPROTO(double complex, catanh, double complex);
LIBSTD_LPROTO(float complex, catanhf, float complex);
LIBSTD_LPROTO(long double complex, catanhl, long double complex);

LIBSTD_LPROTO(double complex, ccosh, double complex);
LIBSTD_LPROTO(float complex, ccoshf, float) complex;
LIBSTD_LPROTO(long double complex, ccoshl, long double complex);

LIBSTD_LPROTO(double complex, csinh, double complex);
LIBSTD_LPROTO(float complex, csinhf, float complex);
LIBSTD_LPROTO(long double complex, csinhl, long double complex);

LIBSTD_LPROTO(double complex, ctanh, double complex);
LIBSTD_LPROTO(float complex, ctanhf, float complex);
LIBSTD_LPROTO(long double complex, ctanhl, long double complex);

LIBSTD_LPROTO(double complex, cexp, double complex);
LIBSTD_LPROTO(float complex, cexpf, float complex);
LIBSTD_LPROTO(long double complex, cexpl, long double complex);

LIBSTD_LPROTO(double complex, clog, double complex);
LIBSTD_LPROTO(float complex, clogf, float complex);
LIBSTD_LPROTO(long double complex, clogl, long double complex);

LIBSTD_LPROTO(double complex, cfabs, double complex);
LIBSTD_LPROTO(float complex, cfabsf, float complex);
LIBSTD_LPROTO(long double complex, cfabsl, long double complex);

LIBSTD_LPROTO(double complex, cpow, double complex, double complex);
LIBSTD_LPROTO(float complex, cpowf, float complex, float complex);
LIBSTD_LPROTO(long double complex, cpowl, long double complex, long double complex);

LIBSTD_LPROTO(double complex, csqrt, double complex);
LIBSTD_LPROTO(float complex, csqrtf, float complex);
LIBSTD_LPROTO(long double complex, csqrtl, long double complex);

LIBSTD_LPROTO(double, carg, double complex);
LIBSTD_LPROTO(float, cargf, float complex);
LIBSTD_LPROTO(long double, cargl, long double complex);

LIBSTD_LPROTO(double, cimag, double complex);
LIBSTD_LPROTO(float, cimagf, float complex);
LIBSTD_LPROTO(long double, cimagl, long double complex);

LIBSTD_LPROTO(double, creal, double complex);
LIBSTD_LPROTO(float, crealf, float complex);
LIBSTD_LPROTO(long double, creall, long double complex);

LIBSTD_LPROTO(double complex, conj, double complex);
LIBSTD_LPROTO(float complex, conjf, float complex);
LIBSTD_LPROTO(long double complex, conjl, long double complex);

LIBSTD_LPROTO(double complex, cproj, double complex);
LIBSTD_LPROTO(float complex, cprojf, float complex);
LIBSTD_LPROTO(long double complex, cprojl, long double complex);

#else /* !__IS_ISO_C99 */

#error  "<complex.h> : Sorry : This header shall only be used with a (C99) compiler supporting _Complex types."

#endif /* __IS_ISO_C99 */

__END_EXTERN_C

#endif
