/* Software IEEE 128-bit floating-point emulation for PowerPC.
   Copyright (C) 2014 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Michael Meissner (meissner@linux.vnet.ibm.com)
   Code is based on the main soft-fp library written by:
	Richard Henderson (rth@cygnus.com) and
	Jakub Jelinek (jj@ultra.linux.cz).

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file into
   combinations with other programs, and to distribute those
   combinations without any restriction coming from the use of this
   file.  (The Lesser General Public License restrictions do apply in
   other respects; for example, they cover modification of the file,
   and distribution when not linked into a combine executable.)

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* Redefine TFtype, so that is __float128 and not long double.  */
#ifndef TFtype
#define TFtype __float128
#endif

/* Use IFtype for IBM long double.  */
#ifndef IFtype
#define IFtype long double
#endif

#include "soft-fp/soft-fp.h"
#include "soft-fp/quad.h"
#include "soft-fp/double.h"
#include "soft-fp/single.h"

#ifndef __FLOAT128__
#error "-mfloat128 is required"
#endif

#if !defined(__LONG_DOUBLE_128__) || defined(__LONG_DOUBLE_IEEE128__)
#error "long double must use the IBM long double format"
#endif

/* If we want to support __float128 on non-VSX systems, we could use #define to
   change all of the functions declared to be a different name.  */

/* Declare all of the support functions.  */
extern TFtype __addkf3 (TFtype, TFtype);
extern TFtype __subkf3 (TFtype, TFtype);
extern TFtype __mulkf3 (TFtype, TFtype);
extern TFtype __divkf3 (TFtype, TFtype);
extern TFtype __negkf2 (TFtype);
extern TFtype __sqrtkf3 (TFtype);

extern CMPtype __cmpkf2 (TFtype, TFtype);
extern CMPtype __eqkf2 (TFtype, TFtype);
extern CMPtype __nekf2 (TFtype, TFtype);
extern CMPtype __ltkf2 (TFtype, TFtype);
extern CMPtype __lekf2 (TFtype, TFtype);
extern CMPtype __gtkf2 (TFtype, TFtype);
extern CMPtype __gekf2 (TFtype, TFtype);
extern CMPtype __uneqkf2 (TFtype, TFtype);
extern CMPtype __ungekf2 (TFtype, TFtype);
extern CMPtype __ungtkf2 (TFtype, TFtype);
extern CMPtype __unlekf2 (TFtype, TFtype);
extern CMPtype __unltkf2 (TFtype, TFtype);
extern CMPtype __ltgtkf2 (TFtype, TFtype);

extern CMPtype __unordkf2 (TFtype, TFtype);
extern CMPtype __orderedkf2 (TFtype, TFtype);

extern TFtype __extenddfkf2 (DFtype);
extern TFtype __extendsfkf2 (SFtype);
extern IFtype __extendkftf2 (TFtype);

extern DFtype __trunckfdf2 (TFtype);
extern SFtype __trunckfsf2 (TFtype);
extern TFtype __trunctfkf2 (IFtype);

extern SItype __fixkfsi (TFtype);
extern DItype __fixkfdi (TFtype);
extern USItype __fixunskfsi (TFtype);
extern UDItype __fixunskfdi (TFtype);

extern TFtype __floatsikf (SItype);
extern TFtype __floatdikf (DItype);

extern TFtype __floatunsikf (USItype);
extern TFtype __floatundikf (UDItype);

#ifdef _ARCH_PPC64
extern TFtype __floattikf (TItype);
extern TFtype __floatuntikf (UTItype);
extern TItype __fixkfti (TFtype);
extern UTItype __fixunskfti (TFtype);
#endif
