/* Software floating-point emulation.
   Definitions for IEEE Quad Precision on the PowerPC.
   Copyright (C) 2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Michael Meissner (meissner@linux.vnet.ibm.com).

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

/* quad.h defines the TFtype type by:
   typedef float TFtype __attribute__ ((mode (TF)));

   This define forces it to use KFmode (aka, ieee 128-bit floating point).  */
#define TF KF

/* Force the use of the VSX instruction set.  */
#if defined(_ARCH_PPC) && (!defined(__VSX__) || !defined(__FLOAT128__))
#pragma GCC target ("vsx,float128")
#endif

#include <quad.h>

/* Add prototypes of the library functions created.  In case the appropriate
   int/long types are not declared in scope by the time quad.h is included,
   provide our own version.  */
typedef int	 SItype_ppc  __attribute__ ((__mode__ (__SI__)));
typedef int	 DItype_ppc  __attribute__ ((__mode__ (__DI__)));
typedef unsigned USItype_ppc __attribute__ ((__mode__ (__SI__)));
typedef unsigned UDItype_ppc __attribute__ ((__mode__ (__DI__)));

#ifdef _ARCH_PPC64
typedef int	 TItype_ppc  __attribute__ ((__mode__ (__TI__)));
typedef unsigned UTItype_ppc __attribute__ ((__mode__ (__TI__)));
#endif

extern TFtype __addkf3 (TFtype, TFtype);
extern TFtype __subkf3 (TFtype, TFtype);
extern TFtype __mulkf3 (TFtype, TFtype);
extern TFtype __divkf3 (TFtype, TFtype);
extern TFtype __negkf2 (TFtype);
extern CMPtype __eqkf2 (TFtype, TFtype);
extern CMPtype __nekf2 (TFtype, TFtype);
extern CMPtype __gekf2 (TFtype, TFtype);
extern CMPtype __gtkf2 (TFtype, TFtype);
extern CMPtype __lekf2 (TFtype, TFtype);
extern CMPtype __ltkf2 (TFtype, TFtype);
extern CMPtype __unordkf2 (TFtype, TFtype);
extern CMPtype __cmpukf2 (TFtype, TFtype);
extern TFtype __extendsfkf2 (float);
extern TFtype __extenddfkf2 (double);
extern float __trunckfsf2 (TFtype);
extern double __trunckfdf2 (TFtype);
extern SItype_ppc __fixkfsi (TFtype);
extern DItype_ppc __fixkfdi (TFtype);
extern USItype_ppc __fixunskfsi (TFtype);
extern UDItype_ppc __fixunskfdi (TFtype);
extern TFtype __floatsikf (SItype_ppc);
extern TFtype __floatdikf (DItype_ppc);
extern TFtype __floatunsikf (USItype_ppc);
extern TFtype __floatundikf (UDItype_ppc);

#ifdef _ARCH_PPC64
extern TItype_ppc __fixkfti (TFtype);
extern UTItype_ppc __fixunskfti (TFtype);
extern TFtype __floattikf (TItype_ppc);
extern TFtype __floatuntikf (UTItype_ppc);
#endif
