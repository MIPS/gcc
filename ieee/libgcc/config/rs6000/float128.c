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

#include "float128.h"


/* Add two IEEE 128-bit floating point values.  */

TFtype
F128_MAP(__addkf3,_q_add) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_Q (A, a);
  FP_UNPACK_SEMIRAW_Q (B, b);
  FP_ADD_Q (R, A, B);
  FP_PACK_SEMIRAW_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Subtract two IEEE 128-bit floating point values.  */

TFtype
F128_MAP(__subkf3,_q_sub) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_Q (A, a);
  FP_UNPACK_SEMIRAW_Q (B, b);
  FP_SUB_Q (R, A, B);
  FP_PACK_SEMIRAW_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Multiply two IEEE 128-bit floating point values.  */

TFtype
F128_MAP(__mulkf3,_q_mul) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_Q (A, a);
  FP_UNPACK_Q (B, b);
  FP_MUL_Q (R, A, B);
  FP_PACK_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Divide two IEEE 128-bit floating point values.  */

TFtype
F128_MAP(__divkf3,_q_div) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_Q (A, a);
  FP_UNPACK_Q (B, b);
  FP_DIV_Q (R, A, B);
  FP_PACK_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Negate an IEEE 128-bit floating point value.  */

TFtype
F128_MAP(__negkf2,_q_neg) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_Q (A, a);
  FP_NEG_Q (R, A);
  FP_PACK_SEMIRAW_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Do a square root of an IEEE 128-bit floating point value.  */

TFtype
F128_MAP(__sqrtkf2,_q_sqrt) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_Q (A, a);
  FP_SQRT_Q (R, A);
  FP_PACK_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}


/* Convert single precision floating point to IEEE 128-bit floating point.  */

TFtype
F128_MAP(__extendsfkf2,_q_stoq) (SFtype a)
{
  FP_DECL_EX;
  FP_DECL_S (A);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_S (A, a);
#if (2 * _FP_W_TYPE_SIZE) < _FP_FRACBITS_Q
  FP_EXTEND (Q, S, 4, 1, R, A);
#else
  FP_EXTEND (Q, S, 2, 1, R, A);
#endif
  FP_PACK_RAW_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Convert double precision floating point to IEEE 128-bit floating point.  */

TFtype
F128_MAP(__extenddfkf2,_q_dtoq) (DFtype a)
{
  FP_DECL_EX;
  FP_DECL_D (A);
  FP_DECL_Q (R);
  TFtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_D (A, a);
#if (2 * _FP_W_TYPE_SIZE) < _FP_FRACBITS_Q
  FP_EXTEND (Q, D, 4, 2, R, A);
#else
  FP_EXTEND (Q, D, 2, 1, R, A);
#endif
  FP_PACK_RAW_Q (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}


/* Convert IEEE 128-bit floating point to IBM long double.  */

#if defined(__LONG_DOUBLE_128__) && !defined(__LONG_DOUBLE_IEEE128__)

IFtype
F128_MAP(__extendkftf2,_q_qtot) (TFtype value)
{
  double high, low;

  high = F128_MAP (__trunckfdf2,_q_qtod) (value);
  if (__builtin_isnan (high) || __builtin_isinf (high))
    low = high;

  else
    {
      TFtype high_f128 = F128_MAP (__extenddfkf2,_q_qtod) (high);
      TFtype low_f128 = F128_MAP (__subkf3,_q_sub) (value, high_f128);

      /* If we are on a power6 or newer, we can use copysign to propigate the
	 sign bit so that -0.0Q becomes -0.0L.  If we are on an older platform,
	 we need to handle the sign propigation.  */
#ifdef _ARCH_PWR6
      low = __builtin_copysign (F128_MAP (__trunckfdf2,_q_qtod) (low_f128), high);
#else
      low = F128_MAP (__trunckfdf2,_q_qtod) (low_f128);
      if (high == 0.0 && low == 0.0)
	low = high;		/* just in case it is -0.0 */
      else if (high < 0)
	low = - __builtin_fabs (low);
      else
	low = __builtin_fabs (low);
#endif	/* no copysign */
    }

#ifdef __LITTLE_ENDIAN__
  return __builtin_pack_longdouble (low, high);
#else
  return __builtin_pack_longdouble (high, low);
#endif
}
#endif	/* long double is IBM long double.  */


/* Convert IEEE 128-bit floating point to single precision floating point.  */

SFtype
F128_MAP(__trunckfsf2,_q_qtos) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_S (R);
  SFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_Q (A, a);
#if (2 * _FP_W_TYPE_SIZE) < _FP_FRACBITS_Q
  FP_TRUNC (S, Q, 1, 4, R, A);
#else
  FP_TRUNC (S, Q, 1, 2, R, A);
#endif
  FP_PACK_SEMIRAW_S (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Convert IEEE 128-bit floating point to double precision floating point.  */

DFtype
F128_MAP(__trunckfdf2,_q_qtod) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_D (R);
  DFtype r;

  FP_INIT_ROUNDMODE;
  FP_UNPACK_SEMIRAW_Q (A, a);
#if (2 * _FP_W_TYPE_SIZE) < _FP_FRACBITS_Q
  FP_TRUNC (D, Q, 2, 4, R, A);
#else
  FP_TRUNC (D, Q, 1, 2, R, A);
#endif
  FP_PACK_SEMIRAW_D (r, R);
  FP_HANDLE_EXCEPTIONS;

  return r;
}


/* Convert IBM long double to IEEE 128-bit floating point.  */
#if defined(__LONG_DOUBLE_128__) && !defined(__LONG_DOUBLE_IEEE128__)

TFtype
F128_MAP(__trunctfkf2,_q_ttoq) (IFtype value)
{
  TFtype high_f128, low_f128;
#ifdef __LITTLE_ENDIAN__
  double high = __builtin_unpack_longdouble (value, 0);
  double low = __builtin_unpack_longdouble (value, 1);
#else
  double high = __builtin_unpack_longdouble (value, 1);
  double low = __builtin_unpack_longdouble (value, 0);
#endif

  if (__builtin_isnan (high))
    return (TFtype) __builtin_nan ("");

  if (__builtin_isinf (high))
    return (TFtype) __builtin_inf ();

  high_f128 = F128_MAP (__extenddfkf2,_q_dtoq) (high);

  if (low == 0.0)
    return high_f128;

  low_f128 = F128_MAP (__extenddfkf2,_q_dtoq) (low);
  return F128_MAP (__addkf3,_q_add) (high_f128, low_f128);
}
#endif	/* long double is IBM long double.  */


/* Convert IEEE 128-bit floating point to signed 32-bit integer.  */

SItype
F128_MAP(__fixkfsi,_q_qtoi) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  USItype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_TO_INT_Q (r, A, SI_BITS, 1);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Convert IEEE 128-bit floating point to signed 64-bit integer.  */

DItype
F128_MAP(__fixkfdi,_q_qtoi_d) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  UDItype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_TO_INT_Q (r, A, DI_BITS, 1);
  FP_HANDLE_EXCEPTIONS;

  return r;
}


/* Convert IEEE 128-bit floating point to unsigned 32-bit integer.  */

USItype
F128_MAP(__fixunskfsi,_q_qtou) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  USItype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_TO_INT_Q (r, A, SI_BITS, 0);
  FP_HANDLE_EXCEPTIONS;

  return r;
}

/* Convert IEEE 128-bit floating point to unsigned 64-bit integer.  */

UDItype
F128_MAP(__fixunskfdi,_q_qtou_d) (TFtype a)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  UDItype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_TO_INT_Q (r, A, DI_BITS, 0);
  FP_HANDLE_EXCEPTIONS;

  return r;
}


/* PowerPC condition register bits.  */
#define PPC_UNORDERED		0x1		/* isnan (a) || isnan (b).  */
#define PPC_EQUAL		0x2		/* a == b.  */
#define PPC_GREATER_THEN	0x4		/* a > b.  */
#define PPC_LESS_THEN		0x8		/* a < b.  */

/* Map FP_CMP_Q output to PowerPC condition register bits.  */
#define CMP_UNORDERED		(-2)		/* isnan (a) || isnan (b).  */
#define CMP_LESS_THEN		(-1)		/* a < b.  */
#define CMP_EQUAL		0		/* a == b.  */
#define CMP_GREATER_THEN	1		/* a < b.  */
#define CMP_INVALID		2		/* raise invalid exception.  */

#define CMP_LOW			CMP_UNORDERED	/* comparison low value.  */
#define CMP_HIGH		CMP_INVALID	/* comparison high value.  */

static const unsigned char ppc_cr_map[] = {
  PPC_UNORDERED,				/* -2: unordered.  */
  PPC_LESS_THEN,				/* -1: a < b.  */
  PPC_EQUAL,					/*  0: a == b.  */
  PPC_GREATER_THEN,				/*  1: a > b.  */
  PPC_UNORDERED,				/*  2: invalid.  */
};

/* Compare two IEEE 128-bit floating point values and return the status.  We
   return the status as a 4-bit value that can be copied into an appropriate
   conditon code register.  */

CMPtype
F128_MAP(__cmpkf2,_q_fcmp) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r < CMP_LOW || r > CMP_HIGH) ? PPC_UNORDERED : ppc_cr_map[r-CMP_LOW];
}


/* Return non-zero if IEEE 128-bit floating point a == b.  */

CMPtype
F128_MAP(__eqkf2,_q_feq) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_EQUAL);
}

/* Return non-zero if IEEE 128-bit floating point a != b.  */

CMPtype
F128_MAP(__nekf2,_q_fne) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r != CMP_EQUAL);
}

/* Return non-zero if IEEE 128-bit floating point a < b.  */

CMPtype
F128_MAP(__ltkf2,_q_flt) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_LESS_THEN);
}

/* Return non-zero if IEEE 128-bit floating point a <= b.  */

CMPtype
F128_MAP(__lekf2,_q_fle) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_EQUAL) || (r == CMP_LESS_THEN);
}

/* Return non-zero if IEEE 128-bit floating point a > b.  */

CMPtype
F128_MAP(__gtkf2,_q_fgt) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_GREATER_THEN);
}

/* Return non-zero if IEEE 128-bit floating point a >= b.  */

CMPtype
F128_MAP(__gekf2,_q_fge) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_EQUAL) || (r == CMP_GREATER_THEN);
}

/* Return non-zero if IEEE 128-bit floating point a == b || isnan (a)
   || isnan (b).  */

CMPtype
F128_MAP(__uneqkf2,_q_funeq) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_EQUAL) || (r == CMP_UNORDERED) || (r == CMP_INVALID);
}

/* Return non-zero if IEEE 128-bit floating point a > b || isnan (a)
   || isnan (b).  */

CMPtype
F128_MAP(__ungtkf2,_q_fungt) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_GREATER_THEN) || (r == CMP_UNORDERED) || (r == CMP_INVALID);
}

/* Return non-zero if IEEE 128-bit floating point a >= b || isnan (a)
   || isnan (b).  */

CMPtype
F128_MAP(__ungekf2,_q_funge) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r != CMP_LESS_THEN);
}

/* Return non-zero if IEEE 128-bit floating point a < b || isnan (a)
   || isnan (b).  */

CMPtype
F128_MAP(__unltkf2,_q_funlt) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_LESS_THEN) || (r == CMP_UNORDERED) || (r == CMP_INVALID);
}

/* Return non-zero if IEEE 128-bit floating point a <= b || isnan (a)
   || isnan (b).  */

CMPtype
F128_MAP(__unlekf2,_q_funle) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r != CMP_LESS_THEN);
}

/* Return non-zero if IEEE 128-bit floating point !isnan (a) && !isnan (b).  */

CMPtype
F128_MAP(__orderedkf2,_q_fordered) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r != CMP_UNORDERED) && (r != CMP_INVALID);
}

/* Return non-zero if IEEE 128-bit floating point isnan (a) || isnan (b).  */

CMPtype
F128_MAP(__unordkf2,_q_funordered) (TFtype a, TFtype b)
{
  FP_DECL_EX;
  FP_DECL_Q (A);
  FP_DECL_Q (B);
  CMPtype r;

  FP_INIT_EXCEPTIONS;
  FP_UNPACK_RAW_Q (A, a);
  FP_UNPACK_RAW_Q (B, b);
  FP_CMP_Q (r, A, B, 2);
  if (r == CMP_INVALID)
    FP_SET_EXCEPTION (FP_EX_INVALID);
  FP_HANDLE_EXCEPTIONS;

  return (r == CMP_UNORDERED) || (r == CMP_INVALID);
}
