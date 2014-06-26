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
__addkf3 (TFtype a, TFtype b)
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
__subkf3 (TFtype a, TFtype b)
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
__mulkf3 (TFtype a, TFtype b)
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
__divkf3 (TFtype a, TFtype b)
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
__negkf3 (TFtype a)
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


/* Convert single precision floating point to IEEE 128-bit floating point.  */

TFtype
__extendsfkf2 (SFtype a)
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
__extenddfkf2 (DFtype a)
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

IFtype
__extendkftf2 (TFtype value)
{
  double high, low;

  high = __trunckfdf2 (value);
  if (__builtin_isnan (high) || __builtin_isinf (high))
    low = high;

  else
    {
      TFtype high_f128 = __extenddfkf2 (high);
      TFtype low_f128 = __subkf3 (value, high_f128);
      low = __builtin_copysign (__trunckfdf2 (low_f128), high);
    }

#ifdef __LITTLE_ENDIAN__
  return __builtin_pack_longdouble (low, high);
#else
  return __builtin_pack_longdouble (high, low);
#endif
}


/* Convert IEEE 128-bit floating point to single precision floating point.  */

SFtype
__trunckfsf2 (TFtype a)
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
__trunckfdf2 (TFtype a)
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

TFtype
__trunctfkf2 (IFtype value)
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

  high_f128 = __extenddfkf2 (high);

  if (low == 0.0)
    return high_f128;

  low_f128 = __extenddfkf2 (low);
  return __addkf3 (high_f128, low_f128);
}


/* Convert IEEE 128-bit floating point to signed 32-bit integer.  */

SItype
__fixkfsi (TFtype a)
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
__fixkfdi (TFtype a)
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
__fixunskfsi (TFtype a)
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
__fixunskfdi (TFtype a)
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
__cmpkf2 (TFtype a, TFtype b)
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
__eqkf2 (TFtype a, TFtype b)
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
__nekf2 (TFtype a, TFtype b)
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
__ltkf2 (TFtype a, TFtype b)
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
__lekf2 (TFtype a, TFtype b)
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
__gtkf2 (TFtype a, TFtype b)
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
__gekf2 (TFtype a, TFtype b)
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

/* Return non-zero if IEEE 128-bit floating point !isnan (a) && !isnan (b).  */

CMPtype
__orderedkf2 (TFtype a, TFtype b)
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
__unordkf2 (TFtype a, TFtype b)
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
