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

#include <quad.h>

/* Add prototypes of the library functions created.  In case the appropriate
   int/long types are not declared in scope by the time quad.h is included,
   provide our own version.  */
typedef int	 SItype_ppc  __attribute__ ((__mode__ (__SI__)));
typedef int	 DItype_ppc  __attribute__ ((__mode__ (__DI__)));
typedef unsigned USItype_ppc __attribute__ ((__mode__ (__SI__)));
typedef unsigned UDItype_ppc __attribute__ ((__mode__ (__DI__)));

TFtype _q_add (TFtype, TFtype);
TFtype _q_sub (TFtype, TFtype);
TFtype _q_mul (TFtype, TFtype);
TFtype _q_div (TFtype, TFtype);
TFtype _q_neg (TFtype);
CMPtype _q_feq (TFtype, TFtype);
CMPtype _q_fge (TFtype, TFtype);
CMPtype _q_fle (TFtype, TFtype);
CMPtype _q_funordered (TFtype, TFtype);
CMPtype _q_fcmp (TFtype, TFtype);
TFtype _q_stoq (float);
TFtype _q_dtoq (double);
float _q_qtos (TFtype);
double _q_qtod (TFtype);
SItype_ppc _q_qtoi (TFtype);
DItype_ppc _q_qtoi_d (TFtype);
USItype_ppc _q_qtou (TFtype);
UDItype_ppc _q_qtou_d (TFtype);
TFtype _q_itoq (SItype_ppc);
TFtype _q_itoq_d (DItype_ppc);
TFtype _q_utoq (USItype_ppc);
TFtype _q_utoq_d (UDItype_ppc);
