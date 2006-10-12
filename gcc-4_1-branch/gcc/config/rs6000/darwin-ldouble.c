/* 128-bit long double support routines for Darwin.
   Copyright (C) 1993, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* Implementations of floating-point long double basic arithmetic
   functions called by the IBM C compiler when generating code for
   PowerPC platforms.  In particular, the following functions are
   implemented: __gcc_qadd, __gcc_qsub, __gcc_qmul, and __gcc_qdiv.
   Double-double algorithms are based on the paper "Doubled-Precision
   IEEE Standard 754 Floating-Point Arithmetic" by W. Kahan, February 26,
   1987.  An alternative published reference is "Software for
   Doubled-Precision Floating-Point Computations", by Seppo Linnainmaa,
   ACM TOMS vol 7 no 3, September 1981, pages 272-283.  */

/* Each long double is made up of two IEEE doubles.  The value of the
   long double is the sum of the values of the two parts.  The most
   significant part is required to be the value of the long double
   rounded to the nearest double, as specified by IEEE.  For Inf
   values, the least significant part is required to be one of +0.0 or
   -0.0.  No other requirements are made; so, for example, 1.0 may be
   represented as (1.0, +0.0) or (1.0, -0.0), and the low part of a
   NaN is don't-care.

   This code currently assumes big-endian.  */

#if ((!defined (__NO_FPRS__) || defined (_SOFT_FLOAT)) \
     && !defined (__LITTLE_ENDIAN__) \
     && (defined (__MACH__) || defined (__powerpc__) || defined (_AIX)))

#define fabs(x) __builtin_fabs(x)
#define isless(x, y) __builtin_isless (x, y)
#define inf() __builtin_inf()

#define unlikely(x) __builtin_expect ((x), 0)

#define nonfinite(a) unlikely (! isless (fabs (a), inf ()))

/* Define ALIASNAME as a strong alias for NAME.  */
# define strong_alias(name, aliasname) _strong_alias(name, aliasname)
# define _strong_alias(name, aliasname) \
  extern __typeof (name) aliasname __attribute__ ((alias (#name)));

/* All these routines actually take two long doubles as parameters,
   but GCC currently generates poor code when a union is used to turn
   a long double into a pair of doubles.  */

long double __gcc_qadd (double, double, double, double);
long double __gcc_qsub (double, double, double, double);
long double __gcc_qmul (double, double, double, double);
long double __gcc_qdiv (double, double, double, double);

#if defined __ELF__ && defined SHARED \
    && (defined __powerpc64__ || !(defined __linux__ || defined __gnu_hurd__))
/* Provide definitions of the old symbol names to satisfy apps and
   shared libs built against an older libgcc.  To access the _xlq
   symbols an explicit version reference is needed, so these won't
   satisfy an unadorned reference like _xlqadd.  If dot symbols are
   not needed, the assembler will remove the aliases from the symbol
   table.  */
__asm__ (".symver __gcc_qadd,_xlqadd@GCC_3.4\n\t"
	 ".symver __gcc_qsub,_xlqsub@GCC_3.4\n\t"
	 ".symver __gcc_qmul,_xlqmul@GCC_3.4\n\t"
	 ".symver __gcc_qdiv,_xlqdiv@GCC_3.4\n\t"
	 ".symver .__gcc_qadd,._xlqadd@GCC_3.4\n\t"
	 ".symver .__gcc_qsub,._xlqsub@GCC_3.4\n\t"
	 ".symver .__gcc_qmul,._xlqmul@GCC_3.4\n\t"
	 ".symver .__gcc_qdiv,._xlqdiv@GCC_3.4");
#endif

typedef union
{
  long double ldval;
  double dval[2];
} longDblUnion;

/* Add two 'long double' values and return the result.	*/
long double
__gcc_qadd (double a, double aa, double c, double cc)
{
  longDblUnion x;
  double z, q, zz, xh;

  z = a + c;

  if (nonfinite (z))
    {
      z = cc + aa + c + a;
      if (nonfinite (z))
	return z;
      x.dval[0] = z;  /* Will always be DBL_MAX.  */
      zz = aa + cc;
      if (fabs(a) > fabs(c))
	x.dval[1] = a - z + c + zz;
      else
	x.dval[1] = c - z + a + zz;
    }
  else
    {
      q = a - z;
      zz = q + c + (a - (q + z)) + aa + cc;

      /* Keep -0 result.  */
      if (zz == 0.0)
	return z;

      xh = z + zz;
      if (nonfinite (xh))
	return xh;

      x.dval[0] = xh;
      x.dval[1] = z - xh + zz;
    }
  return x.ldval;
}

long double
__gcc_qsub (double a, double b, double c, double d)
{
  return __gcc_qadd (a, b, -c, -d);
}

#ifdef _SOFT_FLOAT
static double fmsub (double, double, double);
#endif

long double
__gcc_qmul (double a, double b, double c, double d)
{
  longDblUnion z;
  double t, tau, u, v, w;
  
  t = a * c;			/* Highest order double term.  */

  if (unlikely (t == 0)		/* Preserve -0.  */
      || nonfinite (t))
    return t;

  /* Sum terms of two highest orders. */
  
  /* Use fused multiply-add to get low part of a * c.  */
#ifndef _SOFT_FLOAT
  asm ("fmsub %0,%1,%2,%3" : "=f"(tau) : "f"(a), "f"(c), "f"(t));
#else
  tau = fmsub (a, c, t);
#endif
  v = a*d;
  w = b*c;
  tau += v + w;	    /* Add in other second-order terms.	 */
  u = t + tau;

  /* Construct long double result.  */
  if (nonfinite (u))
    return u;
  z.dval[0] = u;
  z.dval[1] = (t - u) + tau;
  return z.ldval;
}

long double
__gcc_qdiv (double a, double b, double c, double d)
{
  longDblUnion z;
  double s, sigma, t, tau, u, v, w;
  
  t = a / c;                    /* highest order double term */
  
  if (unlikely (t == 0)		/* Preserve -0.  */
      || nonfinite (t))
    return t;

  /* Finite nonzero result requires corrections to the highest order term.  */

  s = c * t;                    /* (s,sigma) = c*t exactly.  */
  w = -(-b + d * t);	/* Written to get fnmsub for speed, but not
			   numerically necessary.  */
  
  /* Use fused multiply-add to get low part of c * t.	 */
#ifndef _SOFT_FLOAT
  asm ("fmsub %0,%1,%2,%3" : "=f"(sigma) : "f"(c), "f"(t), "f"(s));
#else
  sigma = fmsub (c, t, s);
#endif
  v = a - s;
  
  tau = ((v-sigma)+w)/c;   /* Correction to t.  */
  u = t + tau;

  /* Construct long double result.  */
  if (nonfinite (u))
    return u;
  z.dval[0] = u;
  z.dval[1] = (t - u) + tau;
  return z.ldval;
}

#ifdef _SOFT_FLOAT

long double __gcc_qneg (double, double);
int __gcc_qeq (double, double, double, double);
int __gcc_qne (double, double, double, double);
int __gcc_qge (double, double, double, double);
int __gcc_qle (double, double, double, double);
int __gcc_qunord (double, double, double, double);
long double __gcc_stoq (float);
long double __gcc_dtoq (double);
float __gcc_qtos (double, double);
double __gcc_qtod (double, double);
int __gcc_qtoi (double, double);
unsigned int __gcc_qtou (double, double);
long double __gcc_itoq (int);
long double __gcc_utoq (unsigned int);

extern int __eqdf2 (double, double);
extern int __ledf2 (double, double);
extern int __gedf2 (double, double);
extern int __unorddf2 (double, double);

/* Negate 'long double' value and return the result.	*/
long double
__gcc_qneg (double a, double aa)
{
  longDblUnion x;

  x.dval[0] = -a;
  x.dval[1] = -aa;
  return x.ldval;
}

/* Compare two 'long double' values for equality.  */
int
__gcc_qeq (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __eqdf2 (aa, cc);
  return 1;
}

strong_alias (__gcc_qeq, __gcc_qne);

/* Compare two 'long double' values for less than or equal.  */
int
__gcc_qle (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __ledf2 (aa, cc);
  return __ledf2 (a, c);
}

strong_alias (__gcc_qle, __gcc_qlt);

/* Compare two 'long double' values for greater than or equal.  */
int
__gcc_qge (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __gedf2 (aa, cc);
  return __gedf2 (a, c);
}

strong_alias (__gcc_qge, __gcc_qgt);

/* Compare two 'long double' values for unordered.  */
int
__gcc_qunord (double a, double aa, double c, double cc)
{
  if (__eqdf2 (a, c) == 0)
    return __unorddf2 (aa, cc);
  return __unorddf2 (a, c);
}

/* Convert single to long double.  */
long double
__gcc_stoq (float a)
{
  longDblUnion x;

  x.dval[0] = (double) a;
  x.dval[1] = 0.0;

  return x.ldval;
}

/* Convert double to long double.  */
long double
__gcc_dtoq (double a)
{
  longDblUnion x;

  x.dval[0] = a;
  x.dval[1] = 0.0;

  return x.ldval;
}

/* Convert long double to single.  */
float
__gcc_qtos (double a, double aa __attribute__ ((__unused__)))
{
  return (float) a;
}

/* Convert long double to double.  */
double
__gcc_qtod (double a, double aa __attribute__ ((__unused__)))
{
  return a;
}

/* Convert long double to int.  */
int
__gcc_qtoi (double a, double aa)
{
  double z = a + aa;
  return (int) z;
}

/* Convert long double to unsigned int.  */
unsigned int
__gcc_qtou (double a, double aa)
{
  double z = a + aa;
  return (unsigned int) z;
}

/* Convert int to long double.  */
long double
__gcc_itoq (int a)
{
  return __gcc_dtoq ((double) a);
}

/* Convert unsigned int to long double.  */
long double
__gcc_utoq (unsigned int a)
{
  return __gcc_dtoq ((double) a);
}

typedef int QItype __attribute__ ((mode (QI)));
typedef int SItype __attribute__ ((mode (SI)));
typedef int DItype __attribute__ ((mode (DI)));
typedef unsigned int UQItype __attribute__ ((mode (QI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef unsigned int UDItype __attribute__ ((mode (DI)));

typedef unsigned int UHWtype __attribute__ ((mode (HI)));
extern const UQItype __clz_tab[256];
extern void abort (void);
typedef float DFtype __attribute__ ((mode (DF)));

union _FP_UNION_D
{
  DFtype flt;
  struct
  {

    unsigned sign:1;
    unsigned exp:11;
    unsigned frac1:53 - (((unsigned long) 1 << (53 - 1) % 32) != 0) - 32;
    unsigned frac0:32;
  } bits __attribute__ ((packed));
};
typedef float TFtype __attribute__ ((mode (TF)));

union _FP_UNION_Q
{
  TFtype flt;
  struct
  {

    unsigned sign:1;
    unsigned exp:15;
    unsigned long frac3:113 - (((unsigned long) 1 << (113 - 1) % 32) !=
			       0) - (32 * 3);
    unsigned long frac2:32;
    unsigned long frac1:32;
    unsigned long frac0:32;
  } bits __attribute__ ((packed));
};

static double
fmsub (double a, double b, double c)
{
  int _fex = 0;
  long A_c __attribute__ ((unused)), A_s, A_e;
  unsigned long A_f0, A_f1;
  long B_c __attribute__ ((unused)), B_s, B_e;
  unsigned long B_f0, B_f1;
  long C_c __attribute__ ((unused)), C_s, C_e;
  unsigned long C_f0, C_f1;
  long X_c __attribute__ ((unused)), X_s, X_e;
  unsigned long X_f[4];
  long Y_c __attribute__ ((unused)), Y_s, Y_e;
  unsigned long Y_f[4];
  long Z_c __attribute__ ((unused)), Z_s, Z_e;
  unsigned long Z_f[4];
  long U_c __attribute__ ((unused)), U_s, U_e;
  unsigned long U_f[4];
  long V_c __attribute__ ((unused)), V_s, V_e;
  unsigned long V_f[4];
  long R_c __attribute__ ((unused)), R_s, R_e;
  unsigned long R_f0, R_f1;
  double r;
  long double u, v, x, y, z;

/*  FP_INIT_ROUNDMODE */
  do
    {
    }
  while (0);

/*  FP_UNPACK_RAW_D (A, a) */
  do
    {
      union _FP_UNION_D _flo;
      _flo.flt = (a);
      A_f0 = _flo.bits.frac0;
      A_f1 = _flo.bits.frac1;
      A_e = _flo.bits.exp;
      A_s = _flo.bits.sign;
    }
  while (0);

/*  FP_UNPACK_RAW_D (B, b) */
  do
    {
      union _FP_UNION_D _flo;
      _flo.flt = (b);
      B_f0 = _flo.bits.frac0;
      B_f1 = _flo.bits.frac1;
      B_e = _flo.bits.exp;
      B_s = _flo.bits.sign;
    }
  while (0);

/*  FP_UNPACK_RAW_D (C, c) */
  do
    {
      union _FP_UNION_D _flo;
      _flo.flt = (c);
      C_f0 = _flo.bits.frac0;
      C_f1 = _flo.bits.frac1;
      C_e = _flo.bits.exp;
      C_s = _flo.bits.sign;
    }
  while (0);

  /* Extend double to quad.  */
/*  FP_EXTEND(Q,D,4,2,X,A) */
  do
    {
      if (113 < 53 || (32767 - 16383 < 2047 - 1023) || 16383 < 1023 + 53 - 1)
	abort ();
      X_s = A_s;
      do
	{
	  X_f[0] = A_f0;
	  X_f[1] = A_f1;
	  X_f[2] = X_f[3] = 0;
	}
      while (0);
      if ((((A_e + 1) & 2047) > 1))
	{
	  X_e = A_e + 16383 - 1023;
	  do
	    {
	      long _up, _down, _skip, _i;
	      _skip = ((113 - 53)) / 32;
	      _up = ((113 - 53)) % 32;
	      _down = 32 - _up;
	      if (!_up)
		for (_i = 3; _i >= _skip; --_i)
		  X_f[_i] = X_f[_i - _skip];
	      else
		{
		  for (_i = 3; _i > _skip; --_i)
		    X_f[_i] =
		      X_f[_i - _skip] << _up | X_f[_i - _skip - 1] >> _down;
		  X_f[_i--] = X_f[0] << _up;
		}
	      for (; _i >= 0; --_i)
		X_f[_i] = 0;
	    }
	  while (0);
	}
      else
	{
	  if (A_e == 0)
	    {
	      if (((A_f1 | A_f0) == 0))
		X_e = 0;
	      else
		{
		  int _lz;
		  _fex |= (0);
		  do
		    {
		      if (A_f1)
			do
			  {
			    if (sizeof (unsigned long) ==
				sizeof (unsigned int))
			      _lz = __builtin_clz (A_f1);
			    else if (sizeof (unsigned long) ==
				     sizeof (unsigned long))
			      _lz = __builtin_clzl (A_f1);
			    else if (sizeof (unsigned long) ==
				     sizeof (unsigned long long))
			      _lz = __builtin_clzll (A_f1);
			    else
			      abort ();
			  }
			while (0);
		      else
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_lz = __builtin_clz (A_f0);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_lz = __builtin_clzl (A_f0);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_lz = __builtin_clzll (A_f0);
			      else
				abort ();
			    }
			  while (0);
			  _lz += 32;
			}
		    }
		  while (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = (_lz + 113 - (2 * 32)) / 32;
		      _up = (_lz + 113 - (2 * 32)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  X_f[_i] = X_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    X_f[_i] =
			      X_f[_i - _skip] << _up | X_f[_i - _skip -
							   1] >> _down;
			  X_f[_i--] = X_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			X_f[_i] = 0;
		    }
		  while (0);
		  X_e = (16383 - 1023 + 1 + ((2 * 32) - 53) - _lz);
		}
	    }
	  else
	    {
	      X_e = 32767;
	      if (!((A_f1 | A_f0) == 0))
		{
		  if (!((A_f1) & ((unsigned long) 1 << (53 - 2) % 32)))
		    _fex |= (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = ((113 - 53)) / 32;
		      _up = ((113 - 53)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  X_f[_i] = X_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    X_f[_i] =
			      X_f[_i - _skip] << _up | X_f[_i - _skip -
							   1] >> _down;
			  X_f[_i--] = X_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			X_f[_i] = 0;
		    }
		  while (0);
		}
	    }
	}
    }
  while (0);

/*  FP_EXTEND(Q,D,4,2,Y,B) */
  do
    {
      if (113 < 53 || (32767 - 16383 < 2047 - 1023) || 16383 < 1023 + 53 - 1)
	abort ();
      Y_s = B_s;
      do
	{
	  Y_f[0] = B_f0;
	  Y_f[1] = B_f1;
	  Y_f[2] = Y_f[3] = 0;
	}
      while (0);
      if ((((B_e + 1) & 2047) > 1))
	{
	  Y_e = B_e + 16383 - 1023;
	  do
	    {
	      long _up, _down, _skip, _i;
	      _skip = ((113 - 53)) / 32;
	      _up = ((113 - 53)) % 32;
	      _down = 32 - _up;
	      if (!_up)
		for (_i = 3; _i >= _skip; --_i)
		  Y_f[_i] = Y_f[_i - _skip];
	      else
		{
		  for (_i = 3; _i > _skip; --_i)
		    Y_f[_i] =
		      Y_f[_i - _skip] << _up | Y_f[_i - _skip - 1] >> _down;
		  Y_f[_i--] = Y_f[0] << _up;
		}
	      for (; _i >= 0; --_i)
		Y_f[_i] = 0;
	    }
	  while (0);
	}
      else
	{
	  if (B_e == 0)
	    {
	      if (((B_f1 | B_f0) == 0))
		Y_e = 0;
	      else
		{
		  int _lz;
		  _fex |= (0);
		  do
		    {
		      if (B_f1)
			do
			  {
			    if (sizeof (unsigned long) ==
				sizeof (unsigned int))
			      _lz = __builtin_clz (B_f1);
			    else if (sizeof (unsigned long) ==
				     sizeof (unsigned long))
			      _lz = __builtin_clzl (B_f1);
			    else if (sizeof (unsigned long) ==
				     sizeof (unsigned long long))
			      _lz = __builtin_clzll (B_f1);
			    else
			      abort ();
			  }
			while (0);
		      else
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_lz = __builtin_clz (B_f0);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_lz = __builtin_clzl (B_f0);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_lz = __builtin_clzll (B_f0);
			      else
				abort ();
			    }
			  while (0);
			  _lz += 32;
			}
		    }
		  while (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = (_lz + 113 - (2 * 32)) / 32;
		      _up = (_lz + 113 - (2 * 32)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  Y_f[_i] = Y_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    Y_f[_i] =
			      Y_f[_i - _skip] << _up | Y_f[_i - _skip -
							   1] >> _down;
			  Y_f[_i--] = Y_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			Y_f[_i] = 0;
		    }
		  while (0);
		  Y_e = (16383 - 1023 + 1 + ((2 * 32) - 53) - _lz);
		}
	    }
	  else
	    {
	      Y_e = 32767;
	      if (!((B_f1 | B_f0) == 0))
		{
		  if (!((B_f1) & ((unsigned long) 1 << (53 - 2) % 32)))
		    _fex |= (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = ((113 - 53)) / 32;
		      _up = ((113 - 53)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  Y_f[_i] = Y_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    Y_f[_i] =
			      Y_f[_i - _skip] << _up | Y_f[_i - _skip -
							   1] >> _down;
			  Y_f[_i--] = Y_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			Y_f[_i] = 0;
		    }
		  while (0);
		}
	    }
	}
    }
  while (0);

/*  FP_EXTEND(Q,D,4,2,Z,C) */
  do
    {
      if (113 < 53 || (32767 - 16383 < 2047 - 1023) || 16383 < 1023 + 53 - 1)
	abort ();
      Z_s = C_s;
      do
	{
	  Z_f[0] = C_f0;
	  Z_f[1] = C_f1;
	  Z_f[2] = Z_f[3] = 0;
	}
      while (0);
      if ((((C_e + 1) & 2047) > 1))
	{
	  Z_e = C_e + 16383 - 1023;
	  do
	    {
	      long _up, _down, _skip, _i;
	      _skip = ((113 - 53)) / 32;
	      _up = ((113 - 53)) % 32;
	      _down = 32 - _up;
	      if (!_up)
		for (_i = 3; _i >= _skip; --_i)
		  Z_f[_i] = Z_f[_i - _skip];
	      else
		{
		  for (_i = 3; _i > _skip; --_i)
		    Z_f[_i] =
		      Z_f[_i - _skip] << _up | Z_f[_i - _skip - 1] >> _down;
		  Z_f[_i--] = Z_f[0] << _up;
		}
	      for (; _i >= 0; --_i)
		Z_f[_i] = 0;
	    }
	  while (0);
	}
      else
	{
	  if (C_e == 0)
	    {
	      if (((C_f1 | C_f0) == 0))
		Z_e = 0;
	      else
		{
		  int _lz;
		  _fex |= (0);
		  do
		    {
		      if (C_f1)
			do
			  {
			    if (sizeof (unsigned long) ==
				sizeof (unsigned int))
			      _lz = __builtin_clz (C_f1);
			    else if (sizeof (unsigned long) ==
				     sizeof (unsigned long))
			      _lz = __builtin_clzl (C_f1);
			    else if (sizeof (unsigned long) ==
				     sizeof (unsigned long long))
			      _lz = __builtin_clzll (C_f1);
			    else
			      abort ();
			  }
			while (0);
		      else
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_lz = __builtin_clz (C_f0);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_lz = __builtin_clzl (C_f0);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_lz = __builtin_clzll (C_f0);
			      else
				abort ();
			    }
			  while (0);
			  _lz += 32;
			}
		    }
		  while (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = (_lz + 113 - (2 * 32)) / 32;
		      _up = (_lz + 113 - (2 * 32)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  Z_f[_i] = Z_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    Z_f[_i] =
			      Z_f[_i - _skip] << _up | Z_f[_i - _skip -
							   1] >> _down;
			  Z_f[_i--] = Z_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			Z_f[_i] = 0;
		    }
		  while (0);
		  Z_e = (16383 - 1023 + 1 + ((2 * 32) - 53) - _lz);
		}
	    }
	  else
	    {
	      Z_e = 32767;
	      if (!((C_f1 | C_f0) == 0))
		{
		  if (!((C_f1) & ((unsigned long) 1 << (53 - 2) % 32)))
		    _fex |= (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = ((113 - 53)) / 32;
		      _up = ((113 - 53)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  Z_f[_i] = Z_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    Z_f[_i] =
			      Z_f[_i - _skip] << _up | Z_f[_i - _skip -
							   1] >> _down;
			  Z_f[_i--] = Z_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			Z_f[_i] = 0;
		    }
		  while (0);
		}
	    }
	}
    }
  while (0);

/*  FP_PACK_RAW_Q(x,X) */
  do
    {
      union _FP_UNION_Q _flo;
      _flo.bits.frac0 = X_f[0];
      _flo.bits.frac1 = X_f[1];
      _flo.bits.frac2 = X_f[2];
      _flo.bits.frac3 = X_f[3];
      _flo.bits.exp = X_e;
      _flo.bits.sign = X_s;
      (x) = _flo.flt;
    }
  while (0);

/*  FP_PACK_RAW_Q(y,Y) */
  do
    {
      union _FP_UNION_Q _flo;
      _flo.bits.frac0 = Y_f[0];
      _flo.bits.frac1 = Y_f[1];
      _flo.bits.frac2 = Y_f[2];
      _flo.bits.frac3 = Y_f[3];
      _flo.bits.exp = Y_e;
      _flo.bits.sign = Y_s;
      (y) = _flo.flt;
    }
  while (0);

/*  FP_PACK_RAW_Q(z,Z) */
  do
    {
      union _FP_UNION_Q _flo;
      _flo.bits.frac0 = Z_f[0];
      _flo.bits.frac1 = Z_f[1];
      _flo.bits.frac2 = Z_f[2];
      _flo.bits.frac3 = Z_f[3];
      _flo.bits.exp = Z_e;
      _flo.bits.sign = Z_s;
      (z) = _flo.flt;
    }
  while (0);

/*  FP_HANDLE_EXCEPTIONS */
  do
    {
    }
  while (0);

  /* Multiply.  */
/*  FP_INIT_ROUNDMODE */
  do
    {
    }
  while (0);

/*  FP_UNPACK_Q(X,x) */
  do
    {
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.flt = (x);
	  X_f[0] = _flo.bits.frac0;
	  X_f[1] = _flo.bits.frac1;
	  X_f[2] = _flo.bits.frac2;
	  X_f[3] = _flo.bits.frac3;
	  X_e = _flo.bits.exp;
	  X_s = _flo.bits.sign;
	}
      while (0);
      do
	{
	  switch (X_e)
	    {
	    default:
	      (X_f[3]) |= ((unsigned long) 1 << (113 - 1) % 32);
	      do
		{
		  long _up, _down, _skip, _i;
		  _skip = (3) / 32;
		  _up = (3) % 32;
		  _down = 32 - _up;
		  if (!_up)
		    for (_i = 3; _i >= _skip; --_i)
		      X_f[_i] = X_f[_i - _skip];
		  else
		    {
		      for (_i = 3; _i > _skip; --_i)
			X_f[_i] =
			  X_f[_i - _skip] << _up | X_f[_i - _skip -
						       1] >> _down;
		      X_f[_i--] = X_f[0] << _up;
		    }
		  for (; _i >= 0; --_i)
		    X_f[_i] = 0;
		}
	      while (0);
	      X_e -= 16383;
	      X_c = 0;
	      break;
	    case 0:
	      if (((X_f[0] | X_f[1] | X_f[2] | X_f[3]) == 0))
		X_c = 1;
	      else
		{
		  long _shift;
		  do
		    {
		      if (X_f[3])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (X_f[3]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (X_f[3]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (X_f[3]);
			      else
				abort ();
			    }
			  while (0);
			}
		      else if (X_f[2])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (X_f[2]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (X_f[2]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (X_f[2]);
			      else
				abort ();
			    }
			  while (0);
			  _shift += 32;
			}
		      else if (X_f[1])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (X_f[1]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (X_f[1]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (X_f[1]);
			      else
				abort ();
			    }
			  while (0);
			  _shift += 32 * 2;
			}
		      else
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (X_f[0]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (X_f[0]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (X_f[0]);
			      else
				abort ();
			    }
			  while (0);
			  _shift += 32 * 3;
			}
		    }
		  while (0);
		  _shift -= ((4 * 32) - 113);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = ((_shift + 3)) / 32;
		      _up = ((_shift + 3)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  X_f[_i] = X_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    X_f[_i] =
			      X_f[_i - _skip] << _up | X_f[_i - _skip -
							   1] >> _down;
			  X_f[_i--] = X_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			X_f[_i] = 0;
		    }
		  while (0);
		  X_e -= 16383 - 1 + _shift;
		  X_c = 0;
		  _fex |= (0);
		}
	      break;
	    case 32767:
	      if (((X_f[0] | X_f[1] | X_f[2] | X_f[3]) == 0))
		X_c = 2;
	      else
		{
		  X_c = 3;
		  if (!((X_f[3]) & ((unsigned long) 1 << (113 - 2) % 32)))
		    _fex |= (0);
		} break;
	    }
	}
      while (0);
    }
  while (0);

/*  FP_UNPACK_Q(Y,y) */
  do
    {
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.flt = (y);
	  Y_f[0] = _flo.bits.frac0;
	  Y_f[1] = _flo.bits.frac1;
	  Y_f[2] = _flo.bits.frac2;
	  Y_f[3] = _flo.bits.frac3;
	  Y_e = _flo.bits.exp;
	  Y_s = _flo.bits.sign;
	}
      while (0);
      do
	{
	  switch (Y_e)
	    {
	    default:
	      (Y_f[3]) |= ((unsigned long) 1 << (113 - 1) % 32);
	      do
		{
		  long _up, _down, _skip, _i;
		  _skip = (3) / 32;
		  _up = (3) % 32;
		  _down = 32 - _up;
		  if (!_up)
		    for (_i = 3; _i >= _skip; --_i)
		      Y_f[_i] = Y_f[_i - _skip];
		  else
		    {
		      for (_i = 3; _i > _skip; --_i)
			Y_f[_i] =
			  Y_f[_i - _skip] << _up | Y_f[_i - _skip -
						       1] >> _down;
		      Y_f[_i--] = Y_f[0] << _up;
		    }
		  for (; _i >= 0; --_i)
		    Y_f[_i] = 0;
		}
	      while (0);
	      Y_e -= 16383;
	      Y_c = 0;
	      break;
	    case 0:
	      if (((Y_f[0] | Y_f[1] | Y_f[2] | Y_f[3]) == 0))
		Y_c = 1;
	      else
		{
		  long _shift;
		  do
		    {
		      if (Y_f[3])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (Y_f[3]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (Y_f[3]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (Y_f[3]);
			      else
				abort ();
			    }
			  while (0);
			}
		      else if (Y_f[2])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (Y_f[2]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (Y_f[2]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (Y_f[2]);
			      else
				abort ();
			    }
			  while (0);
			  _shift += 32;
			}
		      else if (Y_f[1])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (Y_f[1]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (Y_f[1]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (Y_f[1]);
			      else
				abort ();
			    }
			  while (0);
			  _shift += 32 * 2;
			}
		      else
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				_shift = __builtin_clz (Y_f[0]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				_shift = __builtin_clzl (Y_f[0]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				_shift = __builtin_clzll (Y_f[0]);
			      else
				abort ();
			    }
			  while (0);
			  _shift += 32 * 3;
			}
		    }
		  while (0);
		  _shift -= ((4 * 32) - 113);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = ((_shift + 3)) / 32;
		      _up = ((_shift + 3)) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  Y_f[_i] = Y_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    Y_f[_i] =
			      Y_f[_i - _skip] << _up | Y_f[_i - _skip -
							   1] >> _down;
			  Y_f[_i--] = Y_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			Y_f[_i] = 0;
		    }
		  while (0);
		  Y_e -= 16383 - 1 + _shift;
		  Y_c = 0;
		  _fex |= (0);
		}
	      break;
	    case 32767:
	      if (((Y_f[0] | Y_f[1] | Y_f[2] | Y_f[3]) == 0))
		Y_c = 2;
	      else
		{
		  Y_c = 3;
		  if (!((Y_f[3]) & ((unsigned long) 1 << (113 - 2) % 32)))
		    _fex |= (0);
		} break;
	    }
	}
      while (0);
    }
  while (0);

/*  FP_MUL_Q(U,X,Y) */
  do
    {
      U_s = X_s ^ Y_s;
      switch ((((X_c) << 2) | (Y_c)))
	{
	case (((0) << 2) | (0)):
	  U_c = 0;
	  U_e = X_e + Y_e + 1;
	  do
	    {
	      unsigned long _z_f[8];
	      unsigned long _b_f0, _b_f1;
	      unsigned long _c_f0, _c_f1;
	      unsigned long _d_f0, _d_f1;
	      unsigned long _e_f0, _e_f1;
	      unsigned long _f_f0, _f_f1;
	      do
		{
		  USItype __m0 = (X_f[0]), __m1 = (Y_f[0]);
		__asm__ ("mulhwu %0,%1,%2": "=r" ((_z_f[1])):"%r" (X_f[0]),
			   "r" (Y_f
				[0]));
		  ((_z_f[0])) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[0]), __m1 = (Y_f[1]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_b_f1):"%r" (X_f[0]),
			   "r" (Y_f
				[1]));
		  (_b_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[1]), __m1 = (Y_f[0]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_c_f1):"%r" (X_f[1]),
			   "r" (Y_f
				[0]));
		  (_c_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[1]), __m1 = (Y_f[1]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_d_f1):"%r" (X_f[1]),
			   "r" (Y_f
				[1]));
		  (_d_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[0]), __m1 = (Y_f[2]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_e_f1):"%r" (X_f[0]),
			   "r" (Y_f
				[2]));
		  (_e_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[2]), __m1 = (Y_f[0]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_f_f1):"%r" (X_f[2]),
			   "r" (Y_f
				[0]));
		  (_f_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[1]) = _b_f0 + (_z_f[1]);
		  _c1 = (_z_f[1]) < _b_f0;
		  (_z_f[2]) = _b_f1 + 0;
		  _c2 = (_z_f[2]) < _b_f1;
		  (_z_f[2]) += _c1;
		  _c2 |= (_z_f[2]) < _c1;
		  (_z_f[3]) = 0 + 0 + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[1]) = _c_f0 + (_z_f[1]);
		  _c1 = (_z_f[1]) < _c_f0;
		  (_z_f[2]) = _c_f1 + (_z_f[2]);
		  _c2 = (_z_f[2]) < _c_f1;
		  (_z_f[2]) += _c1;
		  _c2 |= (_z_f[2]) < _c1;
		  (_z_f[3]) = 0 + (_z_f[3]) + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[2]) = _d_f0 + (_z_f[2]);
		  _c1 = (_z_f[2]) < _d_f0;
		  (_z_f[3]) = _d_f1 + (_z_f[3]);
		  _c2 = (_z_f[3]) < _d_f1;
		  (_z_f[3]) += _c1;
		  _c2 |= (_z_f[3]) < _c1;
		  (_z_f[4]) = 0 + 0 + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[2]) = _e_f0 + (_z_f[2]);
		  _c1 = (_z_f[2]) < _e_f0;
		  (_z_f[3]) = _e_f1 + (_z_f[3]);
		  _c2 = (_z_f[3]) < _e_f1;
		  (_z_f[3]) += _c1;
		  _c2 |= (_z_f[3]) < _c1;
		  (_z_f[4]) = 0 + (_z_f[4]) + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[2]) = _f_f0 + (_z_f[2]);
		  _c1 = (_z_f[2]) < _f_f0;
		  (_z_f[3]) = _f_f1 + (_z_f[3]);
		  _c2 = (_z_f[3]) < _f_f1;
		  (_z_f[3]) += _c1;
		  _c2 |= (_z_f[3]) < _c1;
		  (_z_f[4]) = 0 + (_z_f[4]) + _c2;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[0]), __m1 = (Y_f[3]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_b_f1):"%r" (X_f[0]),
			   "r" (Y_f
				[3]));
		  (_b_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[3]), __m1 = (Y_f[0]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_c_f1):"%r" (X_f[3]),
			   "r" (Y_f
				[0]));
		  (_c_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[1]), __m1 = (Y_f[2]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_d_f1):"%r" (X_f[1]),
			   "r" (Y_f
				[2]));
		  (_d_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[2]), __m1 = (Y_f[1]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_e_f1):"%r" (X_f[2]),
			   "r" (Y_f
				[1]));
		  (_e_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[3]) = _b_f0 + (_z_f[3]);
		  _c1 = (_z_f[3]) < _b_f0;
		  (_z_f[4]) = _b_f1 + (_z_f[4]);
		  _c2 = (_z_f[4]) < _b_f1;
		  (_z_f[4]) += _c1;
		  _c2 |= (_z_f[4]) < _c1;
		  (_z_f[5]) = 0 + 0 + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[3]) = _c_f0 + (_z_f[3]);
		  _c1 = (_z_f[3]) < _c_f0;
		  (_z_f[4]) = _c_f1 + (_z_f[4]);
		  _c2 = (_z_f[4]) < _c_f1;
		  (_z_f[4]) += _c1;
		  _c2 |= (_z_f[4]) < _c1;
		  (_z_f[5]) = 0 + (_z_f[5]) + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[3]) = _d_f0 + (_z_f[3]);
		  _c1 = (_z_f[3]) < _d_f0;
		  (_z_f[4]) = _d_f1 + (_z_f[4]);
		  _c2 = (_z_f[4]) < _d_f1;
		  (_z_f[4]) += _c1;
		  _c2 |= (_z_f[4]) < _c1;
		  (_z_f[5]) = 0 + (_z_f[5]) + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[3]) = _e_f0 + (_z_f[3]);
		  _c1 = (_z_f[3]) < _e_f0;
		  (_z_f[4]) = _e_f1 + (_z_f[4]);
		  _c2 = (_z_f[4]) < _e_f1;
		  (_z_f[4]) += _c1;
		  _c2 |= (_z_f[4]) < _c1;
		  (_z_f[5]) = 0 + (_z_f[5]) + _c2;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[2]), __m1 = (Y_f[2]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_b_f1):"%r" (X_f[2]),
			   "r" (Y_f
				[2]));
		  (_b_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[1]), __m1 = (Y_f[3]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_c_f1):"%r" (X_f[1]),
			   "r" (Y_f
				[3]));
		  (_c_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[3]), __m1 = (Y_f[1]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_d_f1):"%r" (X_f[3]),
			   "r" (Y_f
				[1]));
		  (_d_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[2]), __m1 = (Y_f[3]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_e_f1):"%r" (X_f[2]),
			   "r" (Y_f
				[3]));
		  (_e_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[3]), __m1 = (Y_f[2]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_f_f1):"%r" (X_f[3]),
			   "r" (Y_f
				[2]));
		  (_f_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[4]) = _b_f0 + (_z_f[4]);
		  _c1 = (_z_f[4]) < _b_f0;
		  (_z_f[5]) = _b_f1 + (_z_f[5]);
		  _c2 = (_z_f[5]) < _b_f1;
		  (_z_f[5]) += _c1;
		  _c2 |= (_z_f[5]) < _c1;
		  (_z_f[6]) = 0 + 0 + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[4]) = _c_f0 + (_z_f[4]);
		  _c1 = (_z_f[4]) < _c_f0;
		  (_z_f[5]) = _c_f1 + (_z_f[5]);
		  _c2 = (_z_f[5]) < _c_f1;
		  (_z_f[5]) += _c1;
		  _c2 |= (_z_f[5]) < _c1;
		  (_z_f[6]) = 0 + (_z_f[6]) + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[4]) = _d_f0 + (_z_f[4]);
		  _c1 = (_z_f[4]) < _d_f0;
		  (_z_f[5]) = _d_f1 + (_z_f[5]);
		  _c2 = (_z_f[5]) < _d_f1;
		  (_z_f[5]) += _c1;
		  _c2 |= (_z_f[5]) < _c1;
		  (_z_f[6]) = 0 + (_z_f[6]) + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[5]) = _e_f0 + (_z_f[5]);
		  _c1 = (_z_f[5]) < _e_f0;
		  (_z_f[6]) = _e_f1 + (_z_f[6]);
		  _c2 = (_z_f[6]) < _e_f1;
		  (_z_f[6]) += _c1;
		  _c2 |= (_z_f[6]) < _c1;
		  (_z_f[7]) = 0 + 0 + _c2;
		}
	      while (0);
	      do
		{
		  unsigned long _c1, _c2;
		  (_z_f[5]) = _f_f0 + (_z_f[5]);
		  _c1 = (_z_f[5]) < _f_f0;
		  (_z_f[6]) = _f_f1 + (_z_f[6]);
		  _c2 = (_z_f[6]) < _f_f1;
		  (_z_f[6]) += _c1;
		  _c2 |= (_z_f[6]) < _c1;
		  (_z_f[7]) = 0 + (_z_f[7]) + _c2;
		}
	      while (0);
	      do
		{
		  USItype __m0 = (X_f[3]), __m1 = (Y_f[3]);
		__asm__ ("mulhwu %0,%1,%2": "=r" (_b_f1):"%r" (X_f[3]),
			   "r" (Y_f
				[3]));
		  (_b_f0) = __m0 * __m1;
		}
	      while (0);
	      do
		{
		  if (__builtin_constant_p ((_z_f[7])) && ((_z_f[7])) == 0)
		  __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{aze|addze} %0,%2": "=r" ((_z_f[7])), "=&r" ((_z_f[6])):"r" (_b_f1), "%r" (_b_f0),
			     "rI" ((_z_f
				    [6])));
		  else if (__builtin_constant_p ((_z_f[7]))
			   && ((_z_f[7])) == ~(USItype) 0)
		  __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{ame|addme} %0,%2": "=r" ((_z_f[7])), "=&r" ((_z_f[6])):"r" (_b_f1), "%r" (_b_f0),
			     "rI" ((_z_f
				    [6])));
		  else
		  __asm__ ("{a%I5|add%I5c} %1,%4,%5\n\t{ae|adde} %0,%2,%3": "=r" ((_z_f[7])), "=&r" ((_z_f[6])):"%r" (_b_f1), "r" ((_z_f[7])), "%r" (_b_f0),
			     "rI" ((_z_f
				    [6])));
		}
	      while (0);
	      do
		{
		  long _up, _down, _skip, _i;
		  unsigned long _s;
		  _skip = ((3 + 113) - 1) / 32;
		  _down = ((3 + 113) - 1) % 32;
		  _up = 32 - _down;
		  for (_s = _i = 0; _i < _skip; ++_i)
		    _s |= _z_f[_i];
		  if (!_down)
		    for (_i = 0; _i <= 7 - _skip; ++_i)
		      _z_f[_i] = _z_f[_i + _skip];
		  else
		    {
		      _s |= _z_f[_i] << _up;
		      for (_i = 0; _i < 7 - _skip; ++_i)
			_z_f[_i] =
			  _z_f[_i + _skip] >> _down | _z_f[_i + _skip +
							   1] << _up;
		      _z_f[_i++] = _z_f[7] >> _down;
		    }
		  for (; _i < 8; ++_i)
		    _z_f[_i] = 0;
		  _z_f[0] |= (_s != 0);
		}
	      while (0);
	      (U_f[3] = (_z_f[3]), U_f[2] = (_z_f[2]), U_f[1] =
	       (_z_f[1]), U_f[0] = (_z_f[0]));
	    }
	  while (0);
	  if (((U_f[3]) & ((unsigned long) 1 << ((3 + 113) % 32))))
	    do
	      {
		int _sticky;
		do
		  {
		    long _up, _down, _skip, _i;
		    unsigned long _s;
		    _skip = (1) / 32;
		    _down = (1) % 32;
		    _up = 32 - _down;
		    for (_s = _i = 0; _i < _skip; ++_i)
		      _s |= U_f[_i];
		    if (!_down)
		      for (_i = 0; _i <= 3 - _skip; ++_i)
			U_f[_i] = U_f[_i + _skip];
		    else
		      {
			_s |= U_f[_i] << _up;
			for (_i = 0; _i < 3 - _skip; ++_i)
			  U_f[_i] =
			    U_f[_i + _skip] >> _down | U_f[_i + _skip +
							   1] << _up;
			U_f[_i++] = U_f[3] >> _down;
		      }
		    for (; _i < 4; ++_i)
		      U_f[_i] = 0;
		    _sticky = (_s != 0);
		  }
		while (0);
		U_f[0] |= _sticky;
	      }
	    while (0);
	  else
	    U_e--;
	  break;
	case (((3) << 2) | (3)):
	  do
	    {
	      if (((X_f[3]) & ((unsigned long) 1 << (113 - 2) % 32))
		  && !((Y_f[3]) & ((unsigned long) 1 << (113 - 2) % 32)))
		{
		  U_s = Y_s;
		  (U_f[0] = Y_f[0], U_f[1] = Y_f[1], U_f[2] = Y_f[2], U_f[3] =
		   Y_f[3]);
		}
	      else
		{
		  U_s = X_s;
		  (U_f[0] = X_f[0], U_f[1] = X_f[1], U_f[2] = X_f[2], U_f[3] =
		   X_f[3]);
		}
	      U_c = 3;
	    }
	  while (0);
	  break;
	case (((3) << 2) | (0)):
	case (((3) << 2) | (2)):
	case (((3) << 2) | (1)):
	  U_s = X_s;
	case (((2) << 2) | (2)):
	case (((2) << 2) | (0)):
	case (((1) << 2) | (0)):
	case (((1) << 2) | (1)):
	  (U_f[0] = X_f[0], U_f[1] = X_f[1], U_f[2] = X_f[2], U_f[3] =
	   X_f[3]);
	  U_c = X_c;
	  break;
	case (((0) << 2) | (3)):
	case (((2) << 2) | (3)):
	case (((1) << 2) | (3)):
	  U_s = Y_s;
	case (((0) << 2) | (2)):
	case (((0) << 2) | (1)):
	  (U_f[0] = Y_f[0], U_f[1] = Y_f[1], U_f[2] = Y_f[2], U_f[3] =
	   Y_f[3]);
	  U_c = Y_c;
	  break;
	case (((2) << 2) | (1)):
	case (((1) << 2) | (2)):
	  U_s = 0;
	  U_c = 3;
	  (U_f[3] =
	   ((((unsigned long) 1 << (113 - 2) % 32) << 1) - 1), U_f[2] =
	   -1, U_f[1] = -1, U_f[0] = -1);
	  _fex |= (0);
	  break;
	default:
	  abort ();
	}
    }
  while (0);

/*  FP_PACK_Q(u,U) */
  do
    {
      do
	{
	  switch (U_c)
	    {
	    case 0:
	      U_e += 16383;
	      if (U_e > 0)
		{
		  do
		    {
		      if ((U_f[0]) & 7)
			_fex |= (0);
		      switch (0)
			{
			case 0:
			  do
			    {
			      if (((U_f[0]) & 15) != ((unsigned long) 1 << 2))
				do
				  {
				    unsigned long _t;
				    _t =
				      ((U_f[0] +=
					((unsigned long) 1 << 2)) <
				       ((unsigned long) 1 << 2));
				    U_f[1] += _t;
				    _t = (U_f[1] < _t);
				    U_f[2] += _t;
				    _t = (U_f[2] < _t);
				    U_f[3] += _t;
				  }
				while (0);
			    }
			  while (0);
			  break;
			case 1:
			  (void) 0;
			  break;
			case 2:
			  do
			    {
			      if (!U_s && ((U_f[0]) & 7))
				do
				  {
				    unsigned long _t;
				    _t =
				      ((U_f[0] +=
					((unsigned long) 1 << 3)) <
				       ((unsigned long) 1 << 3));
				    U_f[1] += _t;
				    _t = (U_f[1] < _t);
				    U_f[2] += _t;
				    _t = (U_f[2] < _t);
				    U_f[3] += _t;
				  }
				while (0);
			    }
			  while (0);
			  break;
			case 3:
			  do
			    {
			      if (U_s && ((U_f[0]) & 7))
				do
				  {
				    unsigned long _t;
				    _t =
				      ((U_f[0] +=
					((unsigned long) 1 << 3)) <
				       ((unsigned long) 1 << 3));
				    U_f[1] += _t;
				    _t = (U_f[1] < _t);
				    U_f[2] += _t;
				    _t = (U_f[2] < _t);
				    U_f[3] += _t;
				  }
				while (0);
			    }
			  while (0);
			  break;
			}
		    }
		  while (0);
		  if (((U_f[3]) & ((unsigned long) 1 << ((3 + 113) % 32))))
		    {
		      ((U_f[3]) &= ~((unsigned long) 1 << ((3 + 113) % 32)));
		      U_e++;
		    }
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = (3) / 32;
		      _down = (3) % 32;
		      _up = 32 - _down;
		      if (!_down)
			for (_i = 0; _i <= 3 - _skip; ++_i)
			  U_f[_i] = U_f[_i + _skip];
		      else
			{
			  for (_i = 0; _i < 3 - _skip; ++_i)
			    U_f[_i] =
			      U_f[_i + _skip] >> _down | U_f[_i + _skip +
							     1] << _up;
			  U_f[_i++] = U_f[3] >> _down;
			}
		      for (; _i < 4; ++_i)
			U_f[_i] = 0;
		    }
		  while (0);
		  if (U_e >= 32767)
		    {
		      switch (0)
			{
			case 0:
			  U_c = 2;
			  break;
			case 2:
			  if (!U_s)
			    U_c = 2;
			  break;
			case 3:
			  if (U_s)
			    U_c = 2;
			  break;
			}
		      if (U_c == 2)
			{
			  U_e = 32767;
			  (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 0);
			}
		      else
			{
			  U_e = 32767 - 1;
			  (U_f[3] = (~(signed long) 0), U_f[2] =
			   (~(signed long) 0), U_f[1] =
			   (~(signed long) 0), U_f[0] = (~(signed long) 0));
			} _fex |= (0);
		      _fex |= (0);
		    }
		}
	      else
		{
		  U_e = -U_e + 1;
		  if (U_e <= (3 + 113))
		    {
		      do
			{
			  int _sticky;
			  do
			    {
			      long _up, _down, _skip, _i;
			      unsigned long _s;
			      _skip = (U_e) / 32;
			      _down = (U_e) % 32;
			      _up = 32 - _down;
			      for (_s = _i = 0; _i < _skip; ++_i)
				_s |= U_f[_i];
			      if (!_down)
				for (_i = 0; _i <= 3 - _skip; ++_i)
				  U_f[_i] = U_f[_i + _skip];
			      else
				{
				  _s |= U_f[_i] << _up;
				  for (_i = 0; _i < 3 - _skip; ++_i)
				    U_f[_i] =
				      U_f[_i + _skip] >> _down | U_f[_i +
								     _skip +
								     1] <<
				      _up;
				  U_f[_i++] = U_f[3] >> _down;
				}
			      for (; _i < 4; ++_i)
				U_f[_i] = 0;
			      _sticky = (_s != 0);
			    }
			  while (0);
			  U_f[0] |= _sticky;
			}
		      while (0);
		      do
			{
			  if ((U_f[0]) & 7)
			    _fex |= (0);
			  switch (0)
			    {
			    case 0:
			      do
				{
				  if (((U_f[0]) & 15) !=
				      ((unsigned long) 1 << 2))
				    do
				      {
					unsigned long _t;
					_t =
					  ((U_f[0] +=
					    ((unsigned long) 1 << 2)) <
					   ((unsigned long) 1 << 2));
					U_f[1] += _t;
					_t = (U_f[1] < _t);
					U_f[2] += _t;
					_t = (U_f[2] < _t);
					U_f[3] += _t;
				      }
				    while (0);
				}
			      while (0);
			      break;
			    case 1:
			      (void) 0;
			      break;
			    case 2:
			      do
				{
				  if (!U_s && ((U_f[0]) & 7))
				    do
				      {
					unsigned long _t;
					_t =
					  ((U_f[0] +=
					    ((unsigned long) 1 << 3)) <
					   ((unsigned long) 1 << 3));
					U_f[1] += _t;
					_t = (U_f[1] < _t);
					U_f[2] += _t;
					_t = (U_f[2] < _t);
					U_f[3] += _t;
				      }
				    while (0);
				}
			      while (0);
			      break;
			    case 3:
			      do
				{
				  if (U_s && ((U_f[0]) & 7))
				    do
				      {
					unsigned long _t;
					_t =
					  ((U_f[0] +=
					    ((unsigned long) 1 << 3)) <
					   ((unsigned long) 1 << 3));
					U_f[1] += _t;
					_t = (U_f[1] < _t);
					U_f[2] += _t;
					_t = (U_f[2] < _t);
					U_f[3] += _t;
				      }
				    while (0);
				}
			      while (0);
			      break;
			    }
			}
		      while (0);
		      if ((U_f[3]) &
			  (((unsigned long) 1 << ((3 + 113) % 32)) >> 1))
			{
			  U_e = 1;
			  (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 0);
			}
		      else
			{
			  U_e = 0;
			  do
			    {
			      long _up, _down, _skip, _i;
			      _skip = (3) / 32;
			      _down = (3) % 32;
			      _up = 32 - _down;
			      if (!_down)
				for (_i = 0; _i <= 3 - _skip; ++_i)
				  U_f[_i] = U_f[_i + _skip];
			      else
				{
				  for (_i = 0; _i < 3 - _skip; ++_i)
				    U_f[_i] =
				      U_f[_i + _skip] >> _down | U_f[_i +
								     _skip +
								     1] <<
				      _up;
				  U_f[_i++] = U_f[3] >> _down;
				}
			      for (; _i < 4; ++_i)
				U_f[_i] = 0;
			    }
			  while (0);
			  _fex |= (0);
			}
		    }
		  else
		    {
		      U_e = 0;
		      if (!((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			{
			  (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 1);
			  do
			    {
			      if ((U_f[0]) & 7)
				_fex |= (0);
			      switch (0)
				{
				case 0:
				  do
				    {
				      if (((U_f[0]) & 15) !=
					  ((unsigned long) 1 << 2))
					do
					  {
					    unsigned long _t;
					    _t =
					      ((U_f[0] +=
						((unsigned long) 1 << 2)) <
					       ((unsigned long) 1 << 2));
					    U_f[1] += _t;
					    _t = (U_f[1] < _t);
					    U_f[2] += _t;
					    _t = (U_f[2] < _t);
					    U_f[3] += _t;
					  }
					while (0);
				    }
				  while (0);
				  break;
				case 1:
				  (void) 0;
				  break;
				case 2:
				  do
				    {
				      if (!U_s && ((U_f[0]) & 7))
					do
					  {
					    unsigned long _t;
					    _t =
					      ((U_f[0] +=
						((unsigned long) 1 << 3)) <
					       ((unsigned long) 1 << 3));
					    U_f[1] += _t;
					    _t = (U_f[1] < _t);
					    U_f[2] += _t;
					    _t = (U_f[2] < _t);
					    U_f[3] += _t;
					  }
					while (0);
				    }
				  while (0);
				  break;
				case 3:
				  do
				    {
				      if (U_s && ((U_f[0]) & 7))
					do
					  {
					    unsigned long _t;
					    _t =
					      ((U_f[0] +=
						((unsigned long) 1 << 3)) <
					       ((unsigned long) 1 << 3));
					    U_f[1] += _t;
					    _t = (U_f[1] < _t);
					    U_f[2] += _t;
					    _t = (U_f[2] < _t);
					    U_f[3] += _t;
					  }
					while (0);
				    }
				  while (0);
				  break;
				}
			    }
			  while (0);
			  (U_f[0]) >>= (3);
			}
		      _fex |= (0);
		    }
		}
	      break;
	    case 1:
	      U_e = 0;
	      (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 0);
	      break;
	    case 2:
	      U_e = 32767;
	      (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 0);
	      break;
	    case 3:
	      U_e = 32767;
	      if (!1)
		{
		  (U_f[3] =
		   ((((unsigned long) 1 << (113 - 2) % 32) << 1) - 1),
		   U_f[2] = -1, U_f[1] = -1, U_f[0] = -1);
		  U_s = 0;
		}
	      else
		(U_f[3]) |= ((unsigned long) 1 << (113 - 2) % 32);
	      break;
	    }
	}
      while (0);
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.bits.frac0 = U_f[0];
	  _flo.bits.frac1 = U_f[1];
	  _flo.bits.frac2 = U_f[2];
	  _flo.bits.frac3 = U_f[3];
	  _flo.bits.exp = U_e;
	  _flo.bits.sign = U_s;
	  (u) = _flo.flt;
	}
      while (0);
    }
  while (0);

/*  FP_HANDLE_EXCEPTIONS */
  do
    {
    }
  while (0);

  /* Subtract.  */
/*  FP_INIT_ROUNDMODE */
  do
    {
    }
  while (0);

/*  FP_UNPACK_SEMIRAW_Q(U,u) */
  do
    {
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.flt = (u);
	  U_f[0] = _flo.bits.frac0;
	  U_f[1] = _flo.bits.frac1;
	  U_f[2] = _flo.bits.frac2;
	  U_f[3] = _flo.bits.frac3;
	  U_e = _flo.bits.exp;
	  U_s = _flo.bits.sign;
	}
      while (0);
      do
	{
	  long _up, _down, _skip, _i;
	  _skip = (3) / 32;
	  _up = (3) % 32;
	  _down = 32 - _up;
	  if (!_up)
	    for (_i = 3; _i >= _skip; --_i)
	      U_f[_i] = U_f[_i - _skip];
	  else
	    {
	      for (_i = 3; _i > _skip; --_i)
		U_f[_i] =
		  U_f[_i - _skip] << _up | U_f[_i - _skip - 1] >> _down;
	      U_f[_i--] = U_f[0] << _up;
	    }
	  for (; _i >= 0; --_i)
	    U_f[_i] = 0;
	}
      while (0);
    }
  while (0);

/*  FP_UNPACK_SEMIRAW_Q(Z,z) */
  do
    {
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.flt = (z);
	  Z_f[0] = _flo.bits.frac0;
	  Z_f[1] = _flo.bits.frac1;
	  Z_f[2] = _flo.bits.frac2;
	  Z_f[3] = _flo.bits.frac3;
	  Z_e = _flo.bits.exp;
	  Z_s = _flo.bits.sign;
	}
      while (0);
      do
	{
	  long _up, _down, _skip, _i;
	  _skip = (3) / 32;
	  _up = (3) % 32;
	  _down = 32 - _up;
	  if (!_up)
	    for (_i = 3; _i >= _skip; --_i)
	      Z_f[_i] = Z_f[_i - _skip];
	  else
	    {
	      for (_i = 3; _i > _skip; --_i)
		Z_f[_i] =
		  Z_f[_i - _skip] << _up | Z_f[_i - _skip - 1] >> _down;
	      Z_f[_i--] = Z_f[0] << _up;
	    }
	  for (; _i >= 0; --_i)
	    Z_f[_i] = 0;
	}
      while (0);
    }
  while (0);

/*  FP_SUB_Q(V,U,Z) */
  do
    {
      if (!(Z_e == 32767 && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0)))
	Z_s ^= 1;
      do
	{
	  if (U_s == Z_s)
	    {
	      V_s = U_s;
	      int ediff = U_e - Z_e;
	      if (ediff > 0)
		{
		  V_e = U_e;
		  if (Z_e == 0)
		    {
		      if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
			{
			  do
			    {
			      if (U_e == 32767
				  && !((U_f[0] | U_f[1] | U_f[2] | U_f[3]) ==
				       0)
				  && !((U_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			   U_f[2], V_f[3] = U_f[3]);
			  goto add_done;
			}
		      else
			{
			  _fex |= (0);
			  ediff--;
			  if (ediff == 0)
			    {
			      do
				{
				  unsigned long _c1, _c2, _c3;
				  V_f[0] = U_f[0] + Z_f[0];
				  _c1 = V_f[0] < U_f[0];
				  V_f[1] = U_f[1] + Z_f[1];
				  _c2 = V_f[1] < U_f[1];
				  V_f[1] += _c1;
				  _c2 |= V_f[1] < _c1;
				  V_f[2] = U_f[2] + Z_f[2];
				  _c3 = V_f[2] < U_f[2];
				  V_f[2] += _c2;
				  _c3 |= V_f[2] < _c2;
				  V_f[3] = U_f[3] + Z_f[3] + _c3;
				}
			      while (0);
			      goto add3;
			    }
			  if (U_e == 32767)
			    {
			      do
				{
				  if (U_e == 32767
				      && !((U_f[0] | U_f[1] | U_f[2] | U_f[3])
					   == 0)
				      && !((U_f[3]) &
					   ((unsigned long) 1 << (113 - 2 + 3)
					    % 32)))
				    _fex |= (0);
				}
			      while (0);
			      (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			       U_f[2], V_f[3] = U_f[3]);
			      goto add_done;
			    }
			  goto add1;
			}
		    }
		  else if (U_e == 32767)
		    {
		      do
			{
			  if (U_e == 32767
			      && !((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0)
			      && !((U_f[3]) &
				   ((unsigned long) 1 << (113 - 2 + 3) % 32)))
			    _fex |= (0);
			}
		      while (0);
		      (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
		       U_f[2], V_f[3] = U_f[3]);
		      goto add_done;
		    }
		  (Z_f[3]) |= ((unsigned long) 1 << (113 - 1 + 3) % 32);
		add1:if (ediff <= (3 + 113))
		    do
		      {
			int _sticky;
			do
			  {
			    long _up, _down, _skip, _i;
			    unsigned long _s;
			    _skip = (ediff) / 32;
			    _down = (ediff) % 32;
			    _up = 32 - _down;
			    for (_s = _i = 0; _i < _skip; ++_i)
			      _s |= Z_f[_i];
			    if (!_down)
			      for (_i = 0; _i <= 3 - _skip; ++_i)
				Z_f[_i] = Z_f[_i + _skip];
			    else
			      {
				_s |= Z_f[_i] << _up;
				for (_i = 0; _i < 3 - _skip; ++_i)
				  Z_f[_i] =
				    Z_f[_i + _skip] >> _down | Z_f[_i +
								   _skip +
								   1] << _up;
				Z_f[_i++] = Z_f[3] >> _down;
			      }
			    for (; _i < 4; ++_i)
			      Z_f[_i] = 0;
			    _sticky = (_s != 0);
			  }
			while (0);
			Z_f[0] |= _sticky;
		      }
		    while (0);
		  else if (!((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
		    (Z_f[3] = 0, Z_f[2] = 0, Z_f[1] = 0, Z_f[0] = 1);
		  do
		    {
		      unsigned long _c1, _c2, _c3;
		      V_f[0] = U_f[0] + Z_f[0];
		      _c1 = V_f[0] < U_f[0];
		      V_f[1] = U_f[1] + Z_f[1];
		      _c2 = V_f[1] < U_f[1];
		      V_f[1] += _c1;
		      _c2 |= V_f[1] < _c1;
		      V_f[2] = U_f[2] + Z_f[2];
		      _c3 = V_f[2] < U_f[2];
		      V_f[2] += _c2;
		      _c3 |= V_f[2] < _c2;
		      V_f[3] = U_f[3] + Z_f[3] + _c3;
		    }
		  while (0);
		}
	      else if (ediff < 0)
		{
		  ediff = -ediff;
		  V_e = Z_e;
		  if (U_e == 0)
		    {
		      if (((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			{
			  do
			    {
			      if (Z_e == 32767
				  && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) ==
				       0)
				  && !((Z_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			   Z_f[2], V_f[3] = Z_f[3]);
			  goto add_done;
			}
		      else
			{
			  _fex |= (0);
			  ediff--;
			  if (ediff == 0)
			    {
			      do
				{
				  unsigned long _c1, _c2, _c3;
				  V_f[0] = Z_f[0] + U_f[0];
				  _c1 = V_f[0] < Z_f[0];
				  V_f[1] = Z_f[1] + U_f[1];
				  _c2 = V_f[1] < Z_f[1];
				  V_f[1] += _c1;
				  _c2 |= V_f[1] < _c1;
				  V_f[2] = Z_f[2] + U_f[2];
				  _c3 = V_f[2] < Z_f[2];
				  V_f[2] += _c2;
				  _c3 |= V_f[2] < _c2;
				  V_f[3] = Z_f[3] + U_f[3] + _c3;
				}
			      while (0);
			      goto add3;
			    }
			  if (Z_e == 32767)
			    {
			      do
				{
				  if (Z_e == 32767
				      && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3])
					   == 0)
				      && !((Z_f[3]) &
					   ((unsigned long) 1 << (113 - 2 + 3)
					    % 32)))
				    _fex |= (0);
				}
			      while (0);
			      (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			       Z_f[2], V_f[3] = Z_f[3]);
			      goto add_done;
			    }
			  goto add2;
			}
		    }
		  else if (Z_e == 32767)
		    {
		      do
			{
			  if (Z_e == 32767
			      && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0)
			      && !((Z_f[3]) &
				   ((unsigned long) 1 << (113 - 2 + 3) % 32)))
			    _fex |= (0);
			}
		      while (0);
		      (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
		       Z_f[2], V_f[3] = Z_f[3]);
		      goto add_done;
		    }
		  (U_f[3]) |= ((unsigned long) 1 << (113 - 1 + 3) % 32);
		add2:if (ediff <= (3 + 113))
		    do
		      {
			int _sticky;
			do
			  {
			    long _up, _down, _skip, _i;
			    unsigned long _s;
			    _skip = (ediff) / 32;
			    _down = (ediff) % 32;
			    _up = 32 - _down;
			    for (_s = _i = 0; _i < _skip; ++_i)
			      _s |= U_f[_i];
			    if (!_down)
			      for (_i = 0; _i <= 3 - _skip; ++_i)
				U_f[_i] = U_f[_i + _skip];
			    else
			      {
				_s |= U_f[_i] << _up;
				for (_i = 0; _i < 3 - _skip; ++_i)
				  U_f[_i] =
				    U_f[_i + _skip] >> _down | U_f[_i +
								   _skip +
								   1] << _up;
				U_f[_i++] = U_f[3] >> _down;
			      }
			    for (; _i < 4; ++_i)
			      U_f[_i] = 0;
			    _sticky = (_s != 0);
			  }
			while (0);
			U_f[0] |= _sticky;
		      }
		    while (0);
		  else if (!((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
		    (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 1);
		  do
		    {
		      unsigned long _c1, _c2, _c3;
		      V_f[0] = Z_f[0] + U_f[0];
		      _c1 = V_f[0] < Z_f[0];
		      V_f[1] = Z_f[1] + U_f[1];
		      _c2 = V_f[1] < Z_f[1];
		      V_f[1] += _c1;
		      _c2 |= V_f[1] < _c1;
		      V_f[2] = Z_f[2] + U_f[2];
		      _c3 = V_f[2] < Z_f[2];
		      V_f[2] += _c2;
		      _c3 |= V_f[2] < _c2;
		      V_f[3] = Z_f[3] + U_f[3] + _c3;
		    }
		  while (0);
		}
	      else
		{
		  if (!(((U_e + 1) & 32767) > 1))
		    {
		      if (U_e == 0)
			{
			  V_e = 0;
			  if (((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			    {
			      if (!((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
				_fex |= (0);
			      (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			       Z_f[2], V_f[3] = Z_f[3]);
			      goto add_done;
			    }
			  else if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
			    {
			      _fex |= (0);
			      (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			       U_f[2], V_f[3] = U_f[3]);
			      goto add_done;
			    }
			  else
			    {
			      _fex |= (0);
			      do
				{
				  unsigned long _c1, _c2, _c3;
				  V_f[0] = U_f[0] + Z_f[0];
				  _c1 = V_f[0] < U_f[0];
				  V_f[1] = U_f[1] + Z_f[1];
				  _c2 = V_f[1] < U_f[1];
				  V_f[1] += _c1;
				  _c2 |= V_f[1] < _c1;
				  V_f[2] = U_f[2] + Z_f[2];
				  _c3 = V_f[2] < U_f[2];
				  V_f[2] += _c2;
				  _c3 |= V_f[2] < _c2;
				  V_f[3] = U_f[3] + Z_f[3] + _c3;
				}
			      while (0);
			      if ((V_f[3]) &
				  ((unsigned long) 1 << (113 - 1 + 3) % 32))
				{
				  (V_f[3]) &=
				    ~(unsigned long) ((unsigned long) 1 <<
						      (113 - 1 + 3) % 32);
				  V_e = 1;
				}
			      goto add_done;
			    }
			}
		      else
			{
			  do
			    {
			      if (U_e == 32767
				  && !((U_f[0] | U_f[1] | U_f[2] | U_f[3]) ==
				       0)
				  && !((U_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  do
			    {
			      if (Z_e == 32767
				  && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) ==
				       0)
				  && !((Z_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  V_e = 32767;
			  if (((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			    (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			     Z_f[2], V_f[3] = Z_f[3]);
			  else if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
			    (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			     U_f[2], V_f[3] = U_f[3]);
			  else
			    do
			      {
				do
				  {
				    long _up, _down, _skip, _i;
				    _skip = (3) / 32;
				    _down = (3) % 32;
				    _up = 32 - _down;
				    if (!_down)
				      for (_i = 0; _i <= 3 - _skip; ++_i)
					U_f[_i] = U_f[_i + _skip];
				    else
				      {
					for (_i = 0; _i < 3 - _skip; ++_i)
					  U_f[_i] =
					    U_f[_i +
						_skip] >> _down | U_f[_i +
								      _skip +
								      1] <<
					    _up;
					U_f[_i++] = U_f[3] >> _down;
				      }
				    for (; _i < 4; ++_i)
				      U_f[_i] = 0;
				  }
				while (0);
				do
				  {
				    long _up, _down, _skip, _i;
				    _skip = (3) / 32;
				    _down = (3) % 32;
				    _up = 32 - _down;
				    if (!_down)
				      for (_i = 0; _i <= 3 - _skip; ++_i)
					Z_f[_i] = Z_f[_i + _skip];
				    else
				      {
					for (_i = 0; _i < 3 - _skip; ++_i)
					  Z_f[_i] =
					    Z_f[_i +
						_skip] >> _down | Z_f[_i +
								      _skip +
								      1] <<
					    _up;
					Z_f[_i++] = Z_f[3] >> _down;
				      }
				    for (; _i < 4; ++_i)
				      Z_f[_i] = 0;
				  }
				while (0);
				do
				  {
				    if (((U_f[3]) &
					 ((unsigned long) 1 << (113 - 2) %
					  32))
					&& !((Z_f[3]) &
					     ((unsigned long) 1 << (113 - 2) %
					      32)))
				      {
					V_s = Z_s;
					(V_f[0] = Z_f[0], V_f[1] =
					 Z_f[1], V_f[2] = Z_f[2], V_f[3] =
					 Z_f[3]);
				      }
				    else
				      {
					V_s = U_s;
					(V_f[0] = U_f[0], V_f[1] =
					 U_f[1], V_f[2] = U_f[2], V_f[3] =
					 U_f[3]);
				      }
				    V_c = 3;
				  }
				while (0);
				do
				  {
				    long _up, _down, _skip, _i;
				    _skip = (3) / 32;
				    _up = (3) % 32;
				    _down = 32 - _up;
				    if (!_up)
				      for (_i = 3; _i >= _skip; --_i)
					V_f[_i] = V_f[_i - _skip];
				    else
				      {
					for (_i = 3; _i > _skip; --_i)
					  V_f[_i] =
					    V_f[_i - _skip] << _up | V_f[_i -
									 _skip
									 -
									 1] >>
					    _down;
					V_f[_i--] = V_f[0] << _up;
				      }
				    for (; _i >= 0; --_i)
				      V_f[_i] = 0;
				  }
				while (0);
			      }
			    while (0);
			  goto add_done;
			}
		    }
		  do
		    {
		      unsigned long _c1, _c2, _c3;
		      V_f[0] = U_f[0] + Z_f[0];
		      _c1 = V_f[0] < U_f[0];
		      V_f[1] = U_f[1] + Z_f[1];
		      _c2 = V_f[1] < U_f[1];
		      V_f[1] += _c1;
		      _c2 |= V_f[1] < _c1;
		      V_f[2] = U_f[2] + Z_f[2];
		      _c3 = V_f[2] < U_f[2];
		      V_f[2] += _c2;
		      _c3 |= V_f[2] < _c2;
		      V_f[3] = U_f[3] + Z_f[3] + _c3;
		    }
		  while (0);
		  V_e = U_e + 1;
		  do
		    {
		      int _sticky;
		      do
			{
			  long _up, _down, _skip, _i;
			  unsigned long _s;
			  _skip = (1) / 32;
			  _down = (1) % 32;
			  _up = 32 - _down;
			  for (_s = _i = 0; _i < _skip; ++_i)
			    _s |= V_f[_i];
			  if (!_down)
			    for (_i = 0; _i <= 3 - _skip; ++_i)
			      V_f[_i] = V_f[_i + _skip];
			  else
			    {
			      _s |= V_f[_i] << _up;
			      for (_i = 0; _i < 3 - _skip; ++_i)
				V_f[_i] =
				  V_f[_i + _skip] >> _down | V_f[_i + _skip +
								 1] << _up;
			      V_f[_i++] = V_f[3] >> _down;
			    }
			  for (; _i < 4; ++_i)
			    V_f[_i] = 0;
			  _sticky = (_s != 0);
			}
		      while (0);
		      V_f[0] |= _sticky;
		    }
		  while (0);
		  if (V_e == 32767)
		    do
		      {
			if (0 == 0 || (0 == 2 && !V_s) || (0 == 3 && V_s))
			  {
			    V_e = 32767;
			    (V_f[3] = 0, V_f[2] = 0, V_f[1] = 0, V_f[0] = 0);
			  }
			else
			  {
			    V_e = 32767 - 1;
			    _fex |= (0);
			    _fex |= (0);
			    (V_f[3] = (~(signed long) 0), V_f[2] =
			     (~(signed long) 0), V_f[1] =
			     (~(signed long) 0), V_f[0] = (~(signed long) 0));
			  }
		      }
		    while (0);
		  goto add_done;
		}
	    add3:if ((V_f[3]) &
		  ((unsigned long) 1 << (113 - 1 + 3) % 32))
		{
		  (V_f[3]) &=
		    ~(unsigned long) ((unsigned long) 1 << (113 - 1 + 3) %
				      32);
		  V_e++;
		  do
		    {
		      int _sticky;
		      do
			{
			  long _up, _down, _skip, _i;
			  unsigned long _s;
			  _skip = (1) / 32;
			  _down = (1) % 32;
			  _up = 32 - _down;
			  for (_s = _i = 0; _i < _skip; ++_i)
			    _s |= V_f[_i];
			  if (!_down)
			    for (_i = 0; _i <= 3 - _skip; ++_i)
			      V_f[_i] = V_f[_i + _skip];
			  else
			    {
			      _s |= V_f[_i] << _up;
			      for (_i = 0; _i < 3 - _skip; ++_i)
				V_f[_i] =
				  V_f[_i + _skip] >> _down | V_f[_i + _skip +
								 1] << _up;
			      V_f[_i++] = V_f[3] >> _down;
			    }
			  for (; _i < 4; ++_i)
			    V_f[_i] = 0;
			  _sticky = (_s != 0);
			}
		      while (0);
		      V_f[0] |= _sticky;
		    }
		  while (0);
		  if (V_e == 32767)
		    do
		      {
			if (0 == 0 || (0 == 2 && !V_s) || (0 == 3 && V_s))
			  {
			    V_e = 32767;
			    (V_f[3] = 0, V_f[2] = 0, V_f[1] = 0, V_f[0] = 0);
			  }
			else
			  {
			    V_e = 32767 - 1;
			    _fex |= (0);
			    _fex |= (0);
			    (V_f[3] = (~(signed long) 0), V_f[2] =
			     (~(signed long) 0), V_f[1] =
			     (~(signed long) 0), V_f[0] = (~(signed long) 0));
			  }
		      }
		    while (0);
		}
	    add_done:;
	    }
	  else
	    {
	      int ediff = U_e - Z_e;
	      if (ediff > 0)
		{
		  V_e = U_e;
		  V_s = U_s;
		  if (Z_e == 0)
		    {
		      if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
			{
			  do
			    {
			      if (U_e == 32767
				  && !((U_f[0] | U_f[1] | U_f[2] | U_f[3]) ==
				       0)
				  && !((U_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			   U_f[2], V_f[3] = U_f[3]);
			  goto sub_done;
			}
		      else
			{
			  _fex |= (0);
			  ediff--;
			  if (ediff == 0)
			    {
			      do
				{
				  unsigned long _c1, _c2, _c3;
				  V_f[0] = U_f[0] - Z_f[0];
				  _c1 = V_f[0] > U_f[0];
				  V_f[1] = U_f[1] - Z_f[1];
				  _c2 = V_f[1] > U_f[1];
				  V_f[1] -= _c1;
				  _c2 |= _c1 && (Z_f[1] == U_f[1]);
				  V_f[2] = U_f[2] - Z_f[2];
				  _c3 = V_f[2] > U_f[2];
				  V_f[2] -= _c2;
				  _c3 |= _c2 && (Z_f[2] == U_f[2]);
				  V_f[3] = U_f[3] - Z_f[3] - _c3;
				}
			      while (0);
			      goto sub3;
			    }
			  if (U_e == 32767)
			    {
			      do
				{
				  if (U_e == 32767
				      && !((U_f[0] | U_f[1] | U_f[2] | U_f[3])
					   == 0)
				      && !((U_f[3]) &
					   ((unsigned long) 1 << (113 - 2 + 3)
					    % 32)))
				    _fex |= (0);
				}
			      while (0);
			      (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			       U_f[2], V_f[3] = U_f[3]);
			      goto sub_done;
			    }
			  goto sub1;
			}
		    }
		  else if (U_e == 32767)
		    {
		      do
			{
			  if (U_e == 32767
			      && !((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0)
			      && !((U_f[3]) &
				   ((unsigned long) 1 << (113 - 2 + 3) % 32)))
			    _fex |= (0);
			}
		      while (0);
		      (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
		       U_f[2], V_f[3] = U_f[3]);
		      goto sub_done;
		    }
		  (Z_f[3]) |= ((unsigned long) 1 << (113 - 1 + 3) % 32);
		sub1:if (ediff <= (3 + 113))
		    do
		      {
			int _sticky;
			do
			  {
			    long _up, _down, _skip, _i;
			    unsigned long _s;
			    _skip = (ediff) / 32;
			    _down = (ediff) % 32;
			    _up = 32 - _down;
			    for (_s = _i = 0; _i < _skip; ++_i)
			      _s |= Z_f[_i];
			    if (!_down)
			      for (_i = 0; _i <= 3 - _skip; ++_i)
				Z_f[_i] = Z_f[_i + _skip];
			    else
			      {
				_s |= Z_f[_i] << _up;
				for (_i = 0; _i < 3 - _skip; ++_i)
				  Z_f[_i] =
				    Z_f[_i + _skip] >> _down | Z_f[_i +
								   _skip +
								   1] << _up;
				Z_f[_i++] = Z_f[3] >> _down;
			      }
			    for (; _i < 4; ++_i)
			      Z_f[_i] = 0;
			    _sticky = (_s != 0);
			  }
			while (0);
			Z_f[0] |= _sticky;
		      }
		    while (0);
		  else if (!((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
		    (Z_f[3] = 0, Z_f[2] = 0, Z_f[1] = 0, Z_f[0] = 1);
		  do
		    {
		      unsigned long _c1, _c2, _c3;
		      V_f[0] = U_f[0] - Z_f[0];
		      _c1 = V_f[0] > U_f[0];
		      V_f[1] = U_f[1] - Z_f[1];
		      _c2 = V_f[1] > U_f[1];
		      V_f[1] -= _c1;
		      _c2 |= _c1 && (Z_f[1] == U_f[1]);
		      V_f[2] = U_f[2] - Z_f[2];
		      _c3 = V_f[2] > U_f[2];
		      V_f[2] -= _c2;
		      _c3 |= _c2 && (Z_f[2] == U_f[2]);
		      V_f[3] = U_f[3] - Z_f[3] - _c3;
		    }
		  while (0);
		}
	      else if (ediff < 0)
		{
		  ediff = -ediff;
		  V_e = Z_e;
		  V_s = Z_s;
		  if (U_e == 0)
		    {
		      if (((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			{
			  do
			    {
			      if (Z_e == 32767
				  && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) ==
				       0)
				  && !((Z_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			   Z_f[2], V_f[3] = Z_f[3]);
			  goto sub_done;
			}
		      else
			{
			  _fex |= (0);
			  ediff--;
			  if (ediff == 0)
			    {
			      do
				{
				  unsigned long _c1, _c2, _c3;
				  V_f[0] = Z_f[0] - U_f[0];
				  _c1 = V_f[0] > Z_f[0];
				  V_f[1] = Z_f[1] - U_f[1];
				  _c2 = V_f[1] > Z_f[1];
				  V_f[1] -= _c1;
				  _c2 |= _c1 && (U_f[1] == Z_f[1]);
				  V_f[2] = Z_f[2] - U_f[2];
				  _c3 = V_f[2] > Z_f[2];
				  V_f[2] -= _c2;
				  _c3 |= _c2 && (U_f[2] == Z_f[2]);
				  V_f[3] = Z_f[3] - U_f[3] - _c3;
				}
			      while (0);
			      goto sub3;
			    }
			  if (Z_e == 32767)
			    {
			      do
				{
				  if (Z_e == 32767
				      && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3])
					   == 0)
				      && !((Z_f[3]) &
					   ((unsigned long) 1 << (113 - 2 + 3)
					    % 32)))
				    _fex |= (0);
				}
			      while (0);
			      (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			       Z_f[2], V_f[3] = Z_f[3]);
			      goto sub_done;
			    }
			  goto sub2;
			}
		    }
		  else if (Z_e == 32767)
		    {
		      do
			{
			  if (Z_e == 32767
			      && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0)
			      && !((Z_f[3]) &
				   ((unsigned long) 1 << (113 - 2 + 3) % 32)))
			    _fex |= (0);
			}
		      while (0);
		      (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
		       Z_f[2], V_f[3] = Z_f[3]);
		      goto sub_done;
		    }
		  (U_f[3]) |= ((unsigned long) 1 << (113 - 1 + 3) % 32);
		sub2:if (ediff <= (3 + 113))
		    do
		      {
			int _sticky;
			do
			  {
			    long _up, _down, _skip, _i;
			    unsigned long _s;
			    _skip = (ediff) / 32;
			    _down = (ediff) % 32;
			    _up = 32 - _down;
			    for (_s = _i = 0; _i < _skip; ++_i)
			      _s |= U_f[_i];
			    if (!_down)
			      for (_i = 0; _i <= 3 - _skip; ++_i)
				U_f[_i] = U_f[_i + _skip];
			    else
			      {
				_s |= U_f[_i] << _up;
				for (_i = 0; _i < 3 - _skip; ++_i)
				  U_f[_i] =
				    U_f[_i + _skip] >> _down | U_f[_i +
								   _skip +
								   1] << _up;
				U_f[_i++] = U_f[3] >> _down;
			      }
			    for (; _i < 4; ++_i)
			      U_f[_i] = 0;
			    _sticky = (_s != 0);
			  }
			while (0);
			U_f[0] |= _sticky;
		      }
		    while (0);
		  else if (!((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
		    (U_f[3] = 0, U_f[2] = 0, U_f[1] = 0, U_f[0] = 1);
		  do
		    {
		      unsigned long _c1, _c2, _c3;
		      V_f[0] = Z_f[0] - U_f[0];
		      _c1 = V_f[0] > Z_f[0];
		      V_f[1] = Z_f[1] - U_f[1];
		      _c2 = V_f[1] > Z_f[1];
		      V_f[1] -= _c1;
		      _c2 |= _c1 && (U_f[1] == Z_f[1]);
		      V_f[2] = Z_f[2] - U_f[2];
		      _c3 = V_f[2] > Z_f[2];
		      V_f[2] -= _c2;
		      _c3 |= _c2 && (U_f[2] == Z_f[2]);
		      V_f[3] = Z_f[3] - U_f[3] - _c3;
		    }
		  while (0);
		}
	      else
		{
		  if (!(((U_e + 1) & 32767) > 1))
		    {
		      if (U_e == 0)
			{
			  V_e = 0;
			  if (((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			    {
			      (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
			       Z_f[2], V_f[3] = Z_f[3]);
			      if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
				V_s = (0 == 3);
			      else
				{
				  _fex |= (0);
				  V_s = Z_s;
				}
			      goto sub_done;
			    }
			  else if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
			    {
			      _fex |= (0);
			      (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
			       U_f[2], V_f[3] = U_f[3]);
			      V_s = U_s;
			      goto sub_done;
			    }
			  else
			    {
			      _fex |= (0);
			      do
				{
				  unsigned long _c1, _c2, _c3;
				  V_f[0] = U_f[0] - Z_f[0];
				  _c1 = V_f[0] > U_f[0];
				  V_f[1] = U_f[1] - Z_f[1];
				  _c2 = V_f[1] > U_f[1];
				  V_f[1] -= _c1;
				  _c2 |= _c1 && (Z_f[1] == U_f[1]);
				  V_f[2] = U_f[2] - Z_f[2];
				  _c3 = V_f[2] > U_f[2];
				  V_f[2] -= _c2;
				  _c3 |= _c2 && (Z_f[2] == U_f[2]);
				  V_f[3] = U_f[3] - Z_f[3] - _c3;
				}
			      while (0);
			      V_s = U_s;
			      if ((V_f[3]) &
				  ((unsigned long) 1 << (113 - 1 + 3) % 32))
				{
				  do
				    {
				      unsigned long _c1, _c2, _c3;
				      V_f[0] = Z_f[0] - U_f[0];
				      _c1 = V_f[0] > Z_f[0];
				      V_f[1] = Z_f[1] - U_f[1];
				      _c2 = V_f[1] > Z_f[1];
				      V_f[1] -= _c1;
				      _c2 |= _c1 && (U_f[1] == Z_f[1]);
				      V_f[2] = Z_f[2] - U_f[2];
				      _c3 = V_f[2] > Z_f[2];
				      V_f[2] -= _c2;
				      _c3 |= _c2 && (U_f[2] == Z_f[2]);
				      V_f[3] = Z_f[3] - U_f[3] - _c3;
				    }
				  while (0);
				  V_s = Z_s;
				}
			      else
				if (((V_f[0] | V_f[1] | V_f[2] | V_f[3]) ==
				     0))
				V_s = (0 == 3);
			      goto sub_done;
			    }
			}
		      else
			{
			  do
			    {
			      if (U_e == 32767
				  && !((U_f[0] | U_f[1] | U_f[2] | U_f[3]) ==
				       0)
				  && !((U_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  do
			    {
			      if (Z_e == 32767
				  && !((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) ==
				       0)
				  && !((Z_f[3]) &
				       ((unsigned long) 1 << (113 - 2 + 3) %
					32)))
				_fex |= (0);
			    }
			  while (0);
			  V_e = 32767;
			  if (((U_f[0] | U_f[1] | U_f[2] | U_f[3]) == 0))
			    {
			      if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
				{
				  V_s = 0;
				  (V_f[3] =
				   ((((unsigned long) 1 << (113 - 2) %
				      32) << 1) - 1), V_f[2] = -1, V_f[1] =
				   -1, V_f[0] = -1);
				  do
				    {
				      long _up, _down, _skip, _i;
				      _skip = (3) / 32;
				      _up = (3) % 32;
				      _down = 32 - _up;
				      if (!_up)
					for (_i = 3; _i >= _skip; --_i)
					  V_f[_i] = V_f[_i - _skip];
				      else
					{
					  for (_i = 3; _i > _skip; --_i)
					    V_f[_i] =
					      V_f[_i -
						  _skip] << _up | V_f[_i -
								      _skip -
								      1] >>
					      _down;
					  V_f[_i--] = V_f[0] << _up;
					}
				      for (; _i >= 0; --_i)
					V_f[_i] = 0;
				    }
				  while (0);
				  _fex |= (0);
				}
			      else
				{
				  V_s = Z_s;
				  (V_f[0] = Z_f[0], V_f[1] = Z_f[1], V_f[2] =
				   Z_f[2], V_f[3] = Z_f[3]);
				}
			    }
			  else
			    {
			      if (((Z_f[0] | Z_f[1] | Z_f[2] | Z_f[3]) == 0))
				{
				  V_s = U_s;
				  (V_f[0] = U_f[0], V_f[1] = U_f[1], V_f[2] =
				   U_f[2], V_f[3] = U_f[3]);
				}
			      else
				{
				  do
				    {
				      do
					{
					  long _up, _down, _skip, _i;
					  _skip = (3) / 32;
					  _down = (3) % 32;
					  _up = 32 - _down;
					  if (!_down)
					    for (_i = 0; _i <= 3 - _skip;
						 ++_i)
					      U_f[_i] = U_f[_i + _skip];
					  else
					    {
					      for (_i = 0; _i < 3 - _skip;
						   ++_i)
						U_f[_i] =
						  U_f[_i +
						      _skip] >> _down | U_f[_i
									    +
									    _skip
									    +
									    1]
						  << _up;
					      U_f[_i++] = U_f[3] >> _down;
					    }
					  for (; _i < 4; ++_i)
					    U_f[_i] = 0;
					}
				      while (0);
				      do
					{
					  long _up, _down, _skip, _i;
					  _skip = (3) / 32;
					  _down = (3) % 32;
					  _up = 32 - _down;
					  if (!_down)
					    for (_i = 0; _i <= 3 - _skip;
						 ++_i)
					      Z_f[_i] = Z_f[_i + _skip];
					  else
					    {
					      for (_i = 0; _i < 3 - _skip;
						   ++_i)
						Z_f[_i] =
						  Z_f[_i +
						      _skip] >> _down | Z_f[_i
									    +
									    _skip
									    +
									    1]
						  << _up;
					      Z_f[_i++] = Z_f[3] >> _down;
					    }
					  for (; _i < 4; ++_i)
					    Z_f[_i] = 0;
					}
				      while (0);
				      do
					{
					  if (((U_f[3]) &
					       ((unsigned long) 1 << (113 - 2)
						% 32))
					      && !((Z_f[3]) &
						   ((unsigned long) 1 <<
						    (113 - 2) % 32)))
					    {
					      V_s = Z_s;
					      (V_f[0] = Z_f[0], V_f[1] =
					       Z_f[1], V_f[2] =
					       Z_f[2], V_f[3] = Z_f[3]);
					    }
					  else
					    {
					      V_s = U_s;
					      (V_f[0] = U_f[0], V_f[1] =
					       U_f[1], V_f[2] =
					       U_f[2], V_f[3] = U_f[3]);
					    }
					  V_c = 3;
					}
				      while (0);
				      do
					{
					  long _up, _down, _skip, _i;
					  _skip = (3) / 32;
					  _up = (3) % 32;
					  _down = 32 - _up;
					  if (!_up)
					    for (_i = 3; _i >= _skip; --_i)
					      V_f[_i] = V_f[_i - _skip];
					  else
					    {
					      for (_i = 3; _i > _skip; --_i)
						V_f[_i] =
						  V_f[_i -
						      _skip] << _up | V_f[_i -
									  _skip
									  -
									  1]
						  >> _down;
					      V_f[_i--] = V_f[0] << _up;
					    }
					  for (; _i >= 0; --_i)
					    V_f[_i] = 0;
					}
				      while (0);
				    }
				  while (0);
				}
			    }
			  goto sub_done;
			}
		    }
		  V_e = U_e;
		  do
		    {
		      unsigned long _c1, _c2, _c3;
		      V_f[0] = U_f[0] - Z_f[0];
		      _c1 = V_f[0] > U_f[0];
		      V_f[1] = U_f[1] - Z_f[1];
		      _c2 = V_f[1] > U_f[1];
		      V_f[1] -= _c1;
		      _c2 |= _c1 && (Z_f[1] == U_f[1]);
		      V_f[2] = U_f[2] - Z_f[2];
		      _c3 = V_f[2] > U_f[2];
		      V_f[2] -= _c2;
		      _c3 |= _c2 && (Z_f[2] == U_f[2]);
		      V_f[3] = U_f[3] - Z_f[3] - _c3;
		    }
		  while (0);
		  V_s = U_s;
		  if ((V_f[3]) & ((unsigned long) 1 << (113 - 1 + 3) % 32))
		    {
		      do
			{
			  unsigned long _c1, _c2, _c3;
			  V_f[0] = Z_f[0] - U_f[0];
			  _c1 = V_f[0] > Z_f[0];
			  V_f[1] = Z_f[1] - U_f[1];
			  _c2 = V_f[1] > Z_f[1];
			  V_f[1] -= _c1;
			  _c2 |= _c1 && (U_f[1] == Z_f[1]);
			  V_f[2] = Z_f[2] - U_f[2];
			  _c3 = V_f[2] > Z_f[2];
			  V_f[2] -= _c2;
			  _c3 |= _c2 && (U_f[2] == Z_f[2]);
			  V_f[3] = Z_f[3] - U_f[3] - _c3;
			}
		      while (0);
		      V_s = Z_s;
		    }
		  else if (((V_f[0] | V_f[1] | V_f[2] | V_f[3]) == 0))
		    {
		      V_e = 0;
		      V_s = (0 == 3);
		      goto sub_done;
		    }
		  goto norm;
		}
	    sub3:if ((V_f[3]) &
		  ((unsigned long) 1 << (113 - 1 + 3) % 32))
		{
		  int diff;
		  (V_f[3]) &= ((unsigned long) 1 << (113 - 1 + 3) % 32) - 1;
		norm:do
		    {
		      if (V_f[3])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				diff = __builtin_clz (V_f[3]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				diff = __builtin_clzl (V_f[3]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				diff = __builtin_clzll (V_f[3]);
			      else
				abort ();
			    }
			  while (0);
			}
		      else if (V_f[2])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				diff = __builtin_clz (V_f[2]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				diff = __builtin_clzl (V_f[2]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				diff = __builtin_clzll (V_f[2]);
			      else
				abort ();
			    }
			  while (0);
			  diff += 32;
			}
		      else if (V_f[1])
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				diff = __builtin_clz (V_f[1]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				diff = __builtin_clzl (V_f[1]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				diff = __builtin_clzll (V_f[1]);
			      else
				abort ();
			    }
			  while (0);
			  diff += 32 * 2;
			}
		      else
			{
			  do
			    {
			      if (sizeof (unsigned long) ==
				  sizeof (unsigned int))
				diff = __builtin_clz (V_f[0]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long))
				diff = __builtin_clzl (V_f[0]);
			      else if (sizeof (unsigned long) ==
				       sizeof (unsigned long long))
				diff = __builtin_clzll (V_f[0]);
			      else
				abort ();
			    }
			  while (0);
			  diff += 32 * 3;
			}
		    }
		  while (0);
		  diff -= ((4 * 32) - (3 + 113));
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = (diff) / 32;
		      _up = (diff) % 32;
		      _down = 32 - _up;
		      if (!_up)
			for (_i = 3; _i >= _skip; --_i)
			  V_f[_i] = V_f[_i - _skip];
		      else
			{
			  for (_i = 3; _i > _skip; --_i)
			    V_f[_i] =
			      V_f[_i - _skip] << _up | V_f[_i - _skip -
							   1] >> _down;
			  V_f[_i--] = V_f[0] << _up;
			}
		      for (; _i >= 0; --_i)
			V_f[_i] = 0;
		    }
		  while (0);
		  if (V_e <= diff)
		    {
		      diff = diff - V_e + 1;
		      do
			{
			  int _sticky;
			  do
			    {
			      long _up, _down, _skip, _i;
			      unsigned long _s;
			      _skip = (diff) / 32;
			      _down = (diff) % 32;
			      _up = 32 - _down;
			      for (_s = _i = 0; _i < _skip; ++_i)
				_s |= V_f[_i];
			      if (!_down)
				for (_i = 0; _i <= 3 - _skip; ++_i)
				  V_f[_i] = V_f[_i + _skip];
			      else
				{
				  _s |= V_f[_i] << _up;
				  for (_i = 0; _i < 3 - _skip; ++_i)
				    V_f[_i] =
				      V_f[_i + _skip] >> _down | V_f[_i +
								     _skip +
								     1] <<
				      _up;
				  V_f[_i++] = V_f[3] >> _down;
				}
			      for (; _i < 4; ++_i)
				V_f[_i] = 0;
			      _sticky = (_s != 0);
			    }
			  while (0);
			  V_f[0] |= _sticky;
			}
		      while (0);
		      V_e = 0;
		    }
		  else
		    {
		      V_e -= diff;
		      (V_f[3]) &=
			~(unsigned long) ((unsigned long) 1 << (113 - 1 + 3) %
					  32);
		    }
		}
	    sub_done:;
	    }
	}
      while (0);
    }
  while (0);

/*  FP_PACK_SEMIRAW_Q(v,V) */
  do
    {
      do
	{
	  do
	    {
	      if ((V_f[0]) & 7)
		_fex |= (0);
	      switch (0)
		{
		case 0:
		  do
		    {
		      if (((V_f[0]) & 15) != ((unsigned long) 1 << 2))
			do
			  {
			    unsigned long _t;
			    _t =
			      ((V_f[0] +=
				((unsigned long) 1 << 2)) <
			       ((unsigned long) 1 << 2));
			    V_f[1] += _t;
			    _t = (V_f[1] < _t);
			    V_f[2] += _t;
			    _t = (V_f[2] < _t);
			    V_f[3] += _t;
			  }
			while (0);
		    }
		  while (0);
		  break;
		case 1:
		  (void) 0;
		  break;
		case 2:
		  do
		    {
		      if (!V_s && ((V_f[0]) & 7))
			do
			  {
			    unsigned long _t;
			    _t =
			      ((V_f[0] +=
				((unsigned long) 1 << 3)) <
			       ((unsigned long) 1 << 3));
			    V_f[1] += _t;
			    _t = (V_f[1] < _t);
			    V_f[2] += _t;
			    _t = (V_f[2] < _t);
			    V_f[3] += _t;
			  }
			while (0);
		    }
		  while (0);
		  break;
		case 3:
		  do
		    {
		      if (V_s && ((V_f[0]) & 7))
			do
			  {
			    unsigned long _t;
			    _t =
			      ((V_f[0] +=
				((unsigned long) 1 << 3)) <
			       ((unsigned long) 1 << 3));
			    V_f[1] += _t;
			    _t = (V_f[1] < _t);
			    V_f[2] += _t;
			    _t = (V_f[2] < _t);
			    V_f[3] += _t;
			  }
			while (0);
		    }
		  while (0);
		  break;
		}
	    }
	  while (0);
	  if ((V_f[3]) & (((unsigned long) 1 << ((3 + 113) % 32)) >> 1))
	    {
	      (V_f[3]) &= ~(((unsigned long) 1 << ((3 + 113) % 32)) >> 1);
	      V_e++;
	      if (V_e == 32767)
		do
		  {
		    if (0 == 0 || (0 == 2 && !V_s) || (0 == 3 && V_s))
		      {
			V_e = 32767;
			(V_f[3] = 0, V_f[2] = 0, V_f[1] = 0, V_f[0] = 0);
		      }
		    else
		      {
			V_e = 32767 - 1;
			_fex |= (0);
			_fex |= (0);
			(V_f[3] = (~(signed long) 0), V_f[2] =
			 (~(signed long) 0), V_f[1] =
			 (~(signed long) 0), V_f[0] = (~(signed long) 0));
		      }
		  }
		while (0);
	    }
	  do
	    {
	      long _up, _down, _skip, _i;
	      _skip = (3) / 32;
	      _down = (3) % 32;
	      _up = 32 - _down;
	      if (!_down)
		for (_i = 0; _i <= 3 - _skip; ++_i)
		  V_f[_i] = V_f[_i + _skip];
	      else
		{
		  for (_i = 0; _i < 3 - _skip; ++_i)
		    V_f[_i] =
		      V_f[_i + _skip] >> _down | V_f[_i + _skip + 1] << _up;
		  V_f[_i++] = V_f[3] >> _down;
		}
	      for (; _i < 4; ++_i)
		V_f[_i] = 0;
	    }
	  while (0);
	  if (!(((V_e + 1) & 32767) > 1)
	      && !((V_f[0] | V_f[1] | V_f[2] | V_f[3]) == 0))
	    {
	      if (V_e == 0)
		_fex |= (0);
	      else
		{
		  if (!1)
		    {
		      (V_f[3] =
		       ((((unsigned long) 1 << (113 - 2) % 32) << 1) - 1),
		       V_f[2] = -1, V_f[1] = -1, V_f[0] = -1);
		      V_s = 0;
		    }
		  else
		    (V_f[3]) |= ((unsigned long) 1 << (113 - 2) % 32);
		}
	    }
	}
      while (0);
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.bits.frac0 = V_f[0];
	  _flo.bits.frac1 = V_f[1];
	  _flo.bits.frac2 = V_f[2];
	  _flo.bits.frac3 = V_f[3];
	  _flo.bits.exp = V_e;
	  _flo.bits.sign = V_s;
	  (v) = _flo.flt;
	}
      while (0);
    }
  while (0);

/*  FP_HANDLE_EXCEPTIONS */
  do
    {
    }
  while (0);

  /* Truncate quad to double.  */
/*  FP_INIT_ROUNDMODE */
  do
    {
    }
  while (0);

/*  FP_UNPACK_SEMIRAW_Q(V,v) */
  do
    {
      do
	{
	  union _FP_UNION_Q _flo;
	  _flo.flt = (v);
	  V_f[0] = _flo.bits.frac0;
	  V_f[1] = _flo.bits.frac1;
	  V_f[2] = _flo.bits.frac2;
	  V_f[3] = _flo.bits.frac3;
	  V_e = _flo.bits.exp;
	  V_s = _flo.bits.sign;
	}
      while (0);
      do
	{
	  long _up, _down, _skip, _i;
	  _skip = (3) / 32;
	  _up = (3) % 32;
	  _down = 32 - _up;
	  if (!_up)
	    for (_i = 3; _i >= _skip; --_i)
	      V_f[_i] = V_f[_i - _skip];
	  else
	    {
	      for (_i = 3; _i > _skip; --_i)
		V_f[_i] =
		  V_f[_i - _skip] << _up | V_f[_i - _skip - 1] >> _down;
	      V_f[_i--] = V_f[0] << _up;
	    }
	  for (; _i >= 0; --_i)
	    V_f[_i] = 0;
	}
      while (0);
    }
  while (0);

/*  FP_TRUNC(D,Q,2,4,R,V) */
  do
    {
      if (113 < 53 || 16383 < 1023 + 53 - 1)
	abort ();
      R_s = V_s;
      if ((((V_e + 1) & 32767) > 1))
	{
	  R_e = V_e + 1023 - 16383;
	  if (R_e >= 2047)
	    do
	      {
		if (0 == 0 || (0 == 2 && !R_s) || (0 == 3 && R_s))
		  {
		    R_e = 2047;
		    (R_f0 = 0, R_f1 = 0);
		  }
		else
		  {
		    R_e = 2047 - 1;
		    _fex |= (0);
		    _fex |= (0);
		    (R_f0 = (~(signed long) 0), R_f1 = (~(signed long) 0));
		  }
	      }
	    while (0);
	  else
	    {
	      if (R_e <= 0)
		{
		  if (R_e <= 1 - 53)
		    (V_f[3] = 0, V_f[2] = 0, V_f[1] = 0, V_f[0] = 0);
		  else
		    {
		      (V_f[3]) |= ((unsigned long) 1 << (113 - 1 + 3) % 32);
		      do
			{
			  int _sticky;
			  do
			    {
			      long _up, _down, _skip, _i;
			      unsigned long _s;
			      _skip = (((3 + 113) - (3 + 53) + 1 - R_e)) / 32;
			      _down = (((3 + 113) - (3 + 53) + 1 - R_e)) % 32;
			      _up = 32 - _down;
			      for (_s = _i = 0; _i < _skip; ++_i)
				_s |= V_f[_i];
			      if (!_down)
				for (_i = 0; _i <= 3 - _skip; ++_i)
				  V_f[_i] = V_f[_i + _skip];
			      else
				{
				  _s |= V_f[_i] << _up;
				  for (_i = 0; _i < 3 - _skip; ++_i)
				    V_f[_i] =
				      V_f[_i + _skip] >> _down | V_f[_i +
								     _skip +
								     1] <<
				      _up;
				  V_f[_i++] = V_f[3] >> _down;
				}
			      for (; _i < 4; ++_i)
				V_f[_i] = 0;
			      _sticky = (_s != 0);
			    }
			  while (0);
			  V_f[0] |= _sticky;
			}
		      while (0);
		    }
		  R_e = 0;
		}
	      else
		do
		  {
		    int _sticky;
		    do
		      {
			long _up, _down, _skip, _i;
			unsigned long _s;
			_skip = (((3 + 113) - (3 + 53))) / 32;
			_down = (((3 + 113) - (3 + 53))) % 32;
			_up = 32 - _down;
			for (_s = _i = 0; _i < _skip; ++_i)
			  _s |= V_f[_i];
			if (!_down)
			  for (_i = 0; _i <= 3 - _skip; ++_i)
			    V_f[_i] = V_f[_i + _skip];
			else
			  {
			    _s |= V_f[_i] << _up;
			    for (_i = 0; _i < 3 - _skip; ++_i)
			      V_f[_i] =
				V_f[_i + _skip] >> _down | V_f[_i + _skip +
							       1] << _up;
			    V_f[_i++] = V_f[3] >> _down;
			  }
			for (; _i < 4; ++_i)
			  V_f[_i] = 0;
			_sticky = (_s != 0);
		      }
		    while (0);
		    V_f[0] |= _sticky;
		  }
		while (0);
	      do
		{
		  R_f0 = V_f[0];
		  R_f1 = V_f[1];
		}
	      while (0);
	    }
	}
      else
	{
	  if (V_e == 0)
	    {
	      R_e = 0;
	      (R_f0 = 0, R_f1 = 0);
	      if (!((V_f[0] | V_f[1] | V_f[2] | V_f[3]) == 0))
		{
		  _fex |= (0);
		  _fex |= (0);
		}
	    }
	  else
	    {
	      R_e = 2047;
	      if (((V_f[0] | V_f[1] | V_f[2] | V_f[3]) == 0))
		(R_f0 = 0, R_f1 = 0);
	      else
		{
		  do
		    {
		      if (V_e == 32767
			  && !((V_f[0] | V_f[1] | V_f[2] | V_f[3]) == 0)
			  && !((V_f[3]) &
			       ((unsigned long) 1 << (113 - 2 + 3) % 32)))
			_fex |= (0);
		    }
		  while (0);
		  do
		    {
		      long _up, _down, _skip, _i;
		      _skip = (((3 + 113) - (3 + 53))) / 32;
		      _down = (((3 + 113) - (3 + 53))) % 32;
		      _up = 32 - _down;
		      if (!_down)
			for (_i = 0; _i <= 3 - _skip; ++_i)
			  V_f[_i] = V_f[_i + _skip];
		      else
			{
			  for (_i = 0; _i < 3 - _skip; ++_i)
			    V_f[_i] =
			      V_f[_i + _skip] >> _down | V_f[_i + _skip +
							     1] << _up;
			  V_f[_i++] = V_f[3] >> _down;
			}
		      for (; _i < 4; ++_i)
			V_f[_i] = 0;
		    }
		  while (0);
		  do
		    {
		      R_f0 = V_f[0];
		      R_f1 = V_f[1];
		    }
		  while (0);
		  (R_f1) |= ((unsigned long) 1 << (53 - 2 + 3) % 32);
		}
	    }
	}
    }
  while (0);

/*  FP_PACK_SEMIRAW_D(r,R) */
  do
    {
      do
	{
	  do
	    {
	      if ((R_f0) & 7)
		_fex |= (0);
	      switch (0)
		{
		case 0:
		  do
		    {
		      if (((R_f0) & 15) != ((unsigned long) 1 << 2))
			do
			  {
			    if (__builtin_constant_p (0) && (0) == 0)
			    __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{aze|addze} %0,%2": "=r" (R_f1), "=&r" (R_f0):"r" (R_f1), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      2)));
			    else if (__builtin_constant_p (0)
				     && (0) == ~(USItype) 0)
			    __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{ame|addme} %0,%2": "=r" (R_f1), "=&r" (R_f0):"r" (R_f1), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      2)));
			    else
			    __asm__ ("{a%I5|add%I5c} %1,%4,%5\n\t{ae|adde} %0,%2,%3": "=r" (R_f1), "=&r" (R_f0):"%r" (R_f1), "r" (0), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      2)));
			  }
			while (0);
		    }
		  while (0);
		  break;
		case 1:
		  (void) 0;
		  break;
		case 2:
		  do
		    {
		      if (!R_s && ((R_f0) & 7))
			do
			  {
			    if (__builtin_constant_p (0) && (0) == 0)
			    __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{aze|addze} %0,%2": "=r" (R_f1), "=&r" (R_f0):"r" (R_f1), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      3)));
			    else if (__builtin_constant_p (0)
				     && (0) == ~(USItype) 0)
			    __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{ame|addme} %0,%2": "=r" (R_f1), "=&r" (R_f0):"r" (R_f1), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      3)));
			    else
			    __asm__ ("{a%I5|add%I5c} %1,%4,%5\n\t{ae|adde} %0,%2,%3": "=r" (R_f1), "=&r" (R_f0):"%r" (R_f1), "r" (0), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      3)));
			  }
			while (0);
		    }
		  while (0);
		  break;
		case 3:
		  do
		    {
		      if (R_s && ((R_f0) & 7))
			do
			  {
			    if (__builtin_constant_p (0) && (0) == 0)
			    __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{aze|addze} %0,%2": "=r" (R_f1), "=&r" (R_f0):"r" (R_f1), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      3)));
			    else if (__builtin_constant_p (0)
				     && (0) == ~(USItype) 0)
			    __asm__ ("{a%I4|add%I4c} %1,%3,%4\n\t{ame|addme} %0,%2": "=r" (R_f1), "=&r" (R_f0):"r" (R_f1), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      3)));
			    else
			    __asm__ ("{a%I5|add%I5c} %1,%4,%5\n\t{ae|adde} %0,%2,%3": "=r" (R_f1), "=&r" (R_f0):"%r" (R_f1), "r" (0), "%r" (R_f0),
				       "rI" (((unsigned long) 1 <<
					      3)));
			  }
			while (0);
		    }
		  while (0);
		  break;
		}
	    }
	  while (0);
	  if ((R_f1) & (((unsigned long) 1 << (3 + 53) % 32) >> 1))
	    {
	      (R_f1) &= ~(((unsigned long) 1 << (3 + 53) % 32) >> 1);
	      R_e++;
	      if (R_e == 2047)
		do
		  {
		    if (0 == 0 || (0 == 2 && !R_s) || (0 == 3 && R_s))
		      {
			R_e = 2047;
			(R_f0 = 0, R_f1 = 0);
		      }
		    else
		      {
			R_e = 2047 - 1;
			_fex |= (0);
			_fex |= (0);
			(R_f0 = (~(signed long) 0), R_f1 =
			 (~(signed long) 0));
		      }
		  }
		while (0);
	    }
	  (void) (((3) < 32) ? (
				 {
	R_f0 = R_f0 >> (3) | R_f1 << (32 - (3)); R_f1 >>= (3);}):(
				   {
				   R_f0 = R_f1 >> ((3) - 32); R_f1 = 0;}));
	  if (!(((R_e + 1) & 2047) > 1) && !((R_f1 | R_f0) == 0))
	    {
	      if (R_e == 0)
		_fex |= (0);
	      else
		{
		  if (!1)
		    {
		      (R_f0 = -1, R_f1 =
		       ((((unsigned long) 1 << (53 - 2) % 32) << 1) - 1));
		      R_s = 0;
		    }
		  else
		    (R_f1) |= ((unsigned long) 1 << (53 - 2) % 32);
		}
	    }
	}
      while (0);
      do
	{
	  union _FP_UNION_D _flo;
	  _flo.bits.frac0 = R_f0;
	  _flo.bits.frac1 = R_f1;
	  _flo.bits.exp = R_e;
	  _flo.bits.sign = R_s;
	  (r) = _flo.flt;
	}
      while (0);
    }
  while (0);

/*  FP_HANDLE_EXCEPTIONS */
  do
    {
    }
  while (0);

  return r;
}


#endif

#endif
