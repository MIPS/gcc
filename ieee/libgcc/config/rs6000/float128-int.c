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

#define FP_NO_EXCEPTIONS
#include "float128.h"


/* Convert signed 32-bit integer to IEEE 128-bit floating point.  */

TFtype
F128_MAP(__floatsikf,_q_itoq) (SItype i)
{
  FP_DECL_Q (A);
  TFtype a;

  FP_FROM_INT_Q (A, i, SI_BITS, USItype);
  FP_PACK_RAW_Q (a, A);

  return a;
}

/* Convert signed 64-bit integer to IEEE 128-bit floating point.  */

TFtype
F128_MAP(__floatdikf,_q_itoq_d) (DItype i)
{
  FP_DECL_Q (A);
  TFtype a;

  FP_FROM_INT_Q (A, i, DI_BITS, UDItype);
  FP_PACK_RAW_Q (a, A);

  return a;
}


/* Convert unsigned 32-bit integer to IEEE 128-bit floating point.  */

TFtype
F128_MAP(__floatunsikf,_q_utoq) (USItype i)
{
  FP_DECL_Q (A);
  TFtype a;

  FP_FROM_INT_Q (A, i, SI_BITS, USItype);
  FP_PACK_RAW_Q (a, A);

  return a;
}

/* Convert unsigned 64-bit integer to IEEE 128-bit floating point.  */

TFtype
F128_MAP(__floatundikf,_q_utoq_d) (UDItype i)
{
  FP_DECL_Q (A);
  TFtype a;

  FP_FROM_INT_Q (A, i, DI_BITS, UDItype);
  FP_PACK_RAW_Q (a, A);

  return a;
}
