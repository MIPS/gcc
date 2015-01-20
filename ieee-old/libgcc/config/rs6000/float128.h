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
#if defined(__LONG_DOUBLE_128__) && !defined(__LONG_DOUBLE_IEEE128__)
#ifndef IFtype
#define IFtype long double
#endif
#endif

#include "soft-fp/soft-fp.h"
#include "soft-fp/quad.h"
#include "soft-fp/double.h"
#include "soft-fp/single.h"

#ifndef __FLOAT128__
#error "-mfloat128-vsx or -mfloat128-ref is required"
#endif

/* Depending on options, we want to create two different names, one that passes
   the values as VSX types, and the other that passes values by reference.  */
#ifdef __FLOAT128_VSX__
#define F128_MAP(VSX,GPR) VSX
#else
#define F128_MAP(VSX,GPR) GPR
#endif

/* Declare all of the support functions.  */
extern TFtype F128_MAP(__addkf3,_q_add) (TFtype, TFtype);
extern TFtype F128_MAP(__subkf3,_q_sub) (TFtype, TFtype);
extern TFtype F128_MAP(__mulkf3,_q_mul) (TFtype, TFtype);
extern TFtype F128_MAP(__divkf3,_q_div) (TFtype, TFtype);
extern TFtype F128_MAP(__negkf2,_q_neg) (TFtype);
extern TFtype F128_MAP(__sqrtkf2,_q_sqrt) (TFtype);

extern CMPtype F128_MAP(__cmpkf2,_q_fcmp) (TFtype, TFtype);
extern CMPtype F128_MAP(__eqkf2,_q_feq) (TFtype, TFtype);
extern CMPtype F128_MAP(__nekf2,_q_fne) (TFtype, TFtype);
extern CMPtype F128_MAP(__ltkf2,_q_flt) (TFtype, TFtype);
extern CMPtype F128_MAP(__lekf2,_q_fle) (TFtype, TFtype);
extern CMPtype F128_MAP(__gtkf2,_q_fgt) (TFtype, TFtype);
extern CMPtype F128_MAP(__gekf2,_q_fge) (TFtype, TFtype);
extern CMPtype F128_MAP(__uneqkf2,_q_funeq) (TFtype, TFtype);
extern CMPtype F128_MAP(__ungekf2,_q_funge) (TFtype, TFtype);
extern CMPtype F128_MAP(__ungtkf2,_q_fungt) (TFtype, TFtype);
extern CMPtype F128_MAP(__unlekf2,_q_funle) (TFtype, TFtype);
extern CMPtype F128_MAP(__unltkf2,_q_funlt) (TFtype, TFtype);
extern CMPtype F128_MAP(__ltgtkf2,_q_fltgt) (TFtype, TFtype);

extern CMPtype F128_MAP(__unordkf2,_q_funordered) (TFtype, TFtype);
extern CMPtype F128_MAP(__orderedkf2,_q_fordered) (TFtype, TFtype);

extern TFtype F128_MAP(__extenddfkf2,_q_dtoq) (DFtype);
extern TFtype F128_MAP(__extendsfkf2,_q_stoq) (SFtype);

extern DFtype F128_MAP(__trunckfdf2,_q_qtod) (TFtype);
extern SFtype F128_MAP(__trunckfsf2,_q_qtos) (TFtype);

extern SItype F128_MAP(__fixkfsi,_q_qtoi) (TFtype);
extern DItype F128_MAP(__fixkfdi,_q_qtoi_d) (TFtype);
extern USItype F128_MAP(__fixunskfsi,_q_qtou) (TFtype);
extern UDItype F128_MAP(__fixunskfdi,_q_qtou_d) (TFtype);

extern TFtype F128_MAP(__floatsikf,_q_itoq) (SItype);
extern TFtype F128_MAP(__floatdikf,_q_itoq_d) (DItype);

extern TFtype F128_MAP(__floatunsikf,_q_utoq) (USItype);
extern TFtype F128_MAP(__floatundikf,_q_utoq_d) (UDItype);

#if defined(__LONG_DOUBLE_128__) && !defined(__LONG_DOUBLE_IEEE128__)
extern IFtype F128_MAP(__extendkftf2,_q_qtot) (TFtype);
extern TFtype F128_MAP(__trunctfkf2,_q_ttoq) (IFtype);
#endif
