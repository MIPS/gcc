/* This is a software fixed-point library.
   Copyright (C) 2007 Free Software Foundation, Inc.

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

/* This implements fixed-point arithmetic.

   Contributed by Chao-ying Fu  <fu@mips.com>.  */

/* To use this file, we need to define one of the following:
   QQ_MODE, UQQ_MODE, HQ_MODE, UHQ_MODE, SQ_MODE, USQ_MODE, DQ_MODE, UDQ_MODE,
   TQ_MODE, UTQ_MODE, HA_MODE, UHA_MODE, SA_MODE, USA_MODE, DA_MODE, UDA_MODE,
   TA_MODE, UTA_MODE.
   Then, all operators for this machine mode will be created.  */

#include "tconfig.h"
#include "tsystem.h"
#include "coretypes.h"
#include "tm.h"

#ifndef MIN_UNITS_PER_WORD
#define MIN_UNITS_PER_WORD UNITS_PER_WORD
#endif

#include "config/fixed-bit.h"

#if defined(FIXED_ADD)
FIXED_C_TYPE
FIXED_ADD (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  z = x + y;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_ADD */

#if defined(FIXED_SSADD)
FIXED_C_TYPE
FIXED_SSADD (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  z = x + y;
  if ((((x ^ y) >> I_F_BITS) & 1) == 0)
    {
      if (((z ^ x) >> I_F_BITS) & 1)
        {
          z = 1;
          z = z << I_F_BITS;
          if (x >= 0)
            z--;
        }
    }
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_SSADD */

#if defined(FIXED_USADD)
FIXED_C_TYPE
FIXED_USADD (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  z = x + y;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  if (z < x || z < y) /* max */
    {
       z = -1;
#if HAVE_PADDING_BITS
       z = z << PADDING_BITS;
       z = z >> PADDING_BITS;
#endif
    }
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_USADD */

#if defined(FIXED_SUB)
FIXED_C_TYPE
FIXED_SUB (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  z = x - y;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_SUB */

#if defined(FIXED_SSSUB)
FIXED_C_TYPE
FIXED_SSSUB (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  z = x - y;
  if (((x ^ y) >> I_F_BITS) & 1)
    {
      if (((z ^ x) >> I_F_BITS) & 1)
        {
          z = 1;
          z = z << I_F_BITS;
          if (x >= 0)
            z--;
        }
    }
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_SSSUB */

#if defined(FIXED_USSUB)
FIXED_C_TYPE
FIXED_USSUB (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  z = x - y;
  if (x < y)
    z = 0;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_USSUB */

#if defined(FIXED_SATURATE1)
void
FIXED_SATURATE1 (DINT_C_TYPE *a)
{
  DINT_C_TYPE max, min;
  max = (DINT_C_TYPE)1 << I_F_BITS;
  max = max - 1;
#if MODE_UNSIGNED == 0
  min = (DINT_C_TYPE)1 << (2 * FIXED_WIDTH - 1);
  min = min >> (2 * FIXED_WIDTH - 1 - I_F_BITS);
#else
  min = 0;
#endif
  if (*a > max)
    *a = max;
  else if (*a < min)
    *a = min;
}
#endif /* FIXED_SATURATE1 */

#if defined(FIXED_SATURATE2)
void
FIXED_SATURATE2 (INT_C_TYPE *high, INT_C_TYPE *low)
{
  INT_C_TYPE r_max, s_max, r_min, s_min;
  r_max = 0;
#if (MODE_UNSIGNED == 0) || HAVE_PADDING_BITS
  s_max = (INT_C_TYPE)1 << I_F_BITS;
  s_max = s_max - 1;
#else
  s_max = -1;
#endif
#if MODE_UNSIGNED == 0
  r_min = -1;
  s_min = (INT_C_TYPE)1 << (FIXED_WIDTH - 1);
  s_min = s_min >> (FIXED_WIDTH - 1 - I_F_BITS);
#else
  r_min = 0;
  s_min = 0;
#endif

  if (*high > r_max
      || (*high == r_max && (UINT_C_TYPE)(*low) > (UINT_C_TYPE)s_max))
    {
      *high = r_max;
      *low = s_max;
    }
  else if (*high < r_min ||
	   (*high == r_min && (UINT_C_TYPE)(*low) < (UINT_C_TYPE)s_min))
    {
      *high = r_min;
      *low = s_min;
    }
}
#endif /* FIXED_SATURATE2 */

#if defined(FIXED_MULHELPER)
FIXED_C_TYPE
FIXED_MULHELPER (FIXED_C_TYPE a, FIXED_C_TYPE b, word_type satp)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y;

#if defined (DINT_C_TYPE)
  INT_C_TYPE z;
  DINT_C_TYPE dx, dy, dz;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  dx = (DINT_C_TYPE) x;
  dy = (DINT_C_TYPE) y;
  dz = dx * dy;
  dz = dz >> FBITS;

  if (satp)
    FIXED_SATURATE1 (&dz);

  z = (INT_C_TYPE) dz;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;

#else /* No DINT_C_TYPE */
  /* The result of multiplication expands to two INT_C_TYPE.  */
  INTunion aa, bb;
  INTunion a_high, a_low, b_high, b_low;
  INTunion high_high, high_low, low_high, low_low;
  INTunion r, s, temp1, temp2;
  INT_C_TYPE carry = 0;
  INT_C_TYPE z;

  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);

  /* Decompose a and b.  */
  aa.ll = x;
  bb.ll = y;

  a_high.s.low = aa.s.high;
  a_high.s.high = 0;
  a_low.s.low = aa.s.low;
  a_low.s.high = 0;
  b_high.s.low = bb.s.high;
  b_high.s.high = 0;
  b_low.s.low = bb.s.low;
  b_low.s.high = 0;

  /* Perform four multiplications.  */
  low_low.ll = a_low.ll * b_low.ll;
  low_high.ll = a_low.ll * b_high.ll;
  high_low.ll = a_high.ll * b_low.ll;
  high_high.ll = a_high.ll * b_high.ll;

  /* Accumulate four results to {r, s}.  */
  temp1.s.high = high_low.s.low;
  temp1.s.low = 0;
  s.ll = low_low.ll + temp1.ll;
  if ((UINT_C_TYPE) s.ll < (UINT_C_TYPE) low_low.ll
      || (UINT_C_TYPE) s.ll < (UINT_C_TYPE) temp1.ll)
    carry ++; /* Carry.  */
  temp1.ll = s.ll;
  temp2.s.high = low_high.s.low;
  temp2.s.low = 0;
  s.ll = temp1.ll + temp2.ll;
  if ((UINT_C_TYPE) s.ll < (UINT_C_TYPE) temp1.ll
      || (UINT_C_TYPE) s.ll < (UINT_C_TYPE) temp2.ll)
    carry ++; /* Carry.  */

  temp1.s.low = high_low.s.high;
  temp1.s.high = 0;
  r.ll = high_high.ll + temp1.ll;
  temp1.s.low = low_high.s.high;
  temp1.s.high = 0;
  r.ll = r.ll + temp1.ll + carry;

#if MODE_UNSIGNED == 0
  /* For signed types, we need to add neg(y) to r, if x < 0.  */
  if (x < 0)
    r.ll = r.ll - y;
  /* We need to add neg(x) to r, if y < 0.  */
  if (y < 0)
    r.ll = r.ll - x;
#endif

  /* Shift right the result by FBITS.  */
#if FBITS == FIXED_WIDTH
  /* This happens only for unsigned types without any padding bits.
     So, it is safe to set r.ll to 0 as it is logically shifted right.  */
  s.ll = r.ll;
  r.ll = 0;
#else
  s.ll = s.ll >> FBITS;
  temp1.ll = r.ll << (FIXED_WIDTH - FBITS);
  s.ll = s.ll | temp1.ll;
  r.ll = r.ll >> FBITS;
#endif

  if (satp)
    FIXED_SATURATE2 (&r.ll, &s.ll);

  z = (INT_C_TYPE) s.ll;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
#endif
}
#endif /* FIXED_MULHELPER */

#if defined(FIXED_MUL)
FIXED_C_TYPE
FIXED_MUL (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_MULHELPER (a, b, 0);
}
#endif /* FIXED_MUL */

#if defined(FIXED_SSMUL)
FIXED_C_TYPE
FIXED_SSMUL (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_MULHELPER (a, b, 1);
}
#endif /* FIXED_SSMUL */

#if defined(FIXED_USMUL)
FIXED_C_TYPE
FIXED_USMUL (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_MULHELPER (a, b, 1);
}
#endif /* FIXED_USMUL */

#if defined(FIXED_DIVHELPER)
FIXED_C_TYPE
FIXED_DIVHELPER (FIXED_C_TYPE a, FIXED_C_TYPE b, word_type satp)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y;
  INT_C_TYPE z;

#if defined (DINT_C_TYPE)
  DINT_C_TYPE dx, dy, dz;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  dx = (DINT_C_TYPE) x;
  dy = (DINT_C_TYPE) y;
  dx = dx << FBITS;
  dz = dx / dy;
  if (satp)
    FIXED_SATURATE1 (&dz);
  z = (INT_C_TYPE) dz;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;

#else /* No DINT_C_TYPE */
  INT_C_TYPE pos_a, pos_b, r, s;
  INT_C_TYPE quo_r, quo_s, mod, temp;
  word_type i;
#if MODE_UNSIGNED == 0
  word_type num_of_neg = 0;
#endif

  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);
  pos_a = x;
  pos_b = y;

#if MODE_UNSIGNED == 0
  /* If a < 0, negate a.  */
  if (pos_a < 0)
    {
      pos_a = -pos_a;
      num_of_neg ++;
    }
  /* If b < 0, negate b.  */
  if (pos_b < 0)
    {
      pos_b = -pos_b;
      num_of_neg ++;
    }
#endif

  /* Left shift pos_a to {r, s} by FBITS.  */
#if FBITS == FIXED_WIDTH
  /* This happens only for unsigned types without any padding bits.  */
  r = pos_a;
  s = 0;
#else
  s = pos_a << FBITS;
  r = pos_a >> (FIXED_WIDTH - FBITS);
#endif

  /* Unsigned divide r by pos_b to quo_r.  The remanider is in mod.  */
  quo_r = (UINT_C_TYPE)r / (UINT_C_TYPE)pos_b;
  mod = (UINT_C_TYPE)r % (UINT_C_TYPE)pos_b;
  quo_s = 0;

  for (i = 0; i < FIXED_WIDTH; i++)
    {
      /* Record the leftmost bit of mod.  */
      word_type leftmost_mode = (mod >> (FIXED_WIDTH - 1)) & 1;
      /* Shift left mod by 1 bit.  */
      mod = mod << 1;
      /* Test the leftmost bit of s to add to mod.  */
      if ((s >> (FIXED_WIDTH - 1)) & 1)
	mod ++;
      /* Shift left quo_s by 1 bit.  */
      quo_s = quo_s << 1;
      /* Try to calculate (mod - pos_b).  */
      temp = mod - pos_b;
      if (leftmost_mode || (UINT_C_TYPE)mod >= (UINT_C_TYPE)pos_b)
	{
	  quo_s ++;
	  mod = temp;
	}
      /* Shift left s by 1 bit.  */
      s = s << 1;
    }

#if MODE_UNSIGNED == 0
    if (num_of_neg == 1)
      {
	quo_s = -quo_s;
	if (quo_s == 0)
	  quo_r = -quo_r;
	else
	  quo_r = ~quo_r;
      }
#endif
  if (satp)
    FIXED_SATURATE2 (&quo_r, &quo_s);
  z = quo_s;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
#endif
}
#endif /* FIXED_DIVHELPER */

#if defined(FIXED_DIV)
FIXED_C_TYPE
FIXED_DIV (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_DIVHELPER (a, b, 0);
}
#endif /* FIXED_DIV */


#if defined(FIXED_UDIV)
FIXED_C_TYPE
FIXED_UDIV (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_DIVHELPER (a, b, 0);
}
#endif /* FIXED_UDIV */

#if defined(FIXED_SSDIV)
FIXED_C_TYPE
FIXED_SSDIV (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_DIVHELPER (a, b, 1);
}
#endif /* FIXED_SSDIV */

#if defined(FIXED_USDIV)
FIXED_C_TYPE
FIXED_USDIV (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  return FIXED_DIVHELPER (a, b, 1);
}
#endif /* FIXED_USDIV */

#if defined(FIXED_NEG)
FIXED_C_TYPE
FIXED_NEG (FIXED_C_TYPE a)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, z;
  memcpy (&x, &a, FIXED_SIZE);
  z = -x;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_NEG */

#if defined(FIXED_SSNEG)
FIXED_C_TYPE
FIXED_SSNEG (FIXED_C_TYPE a)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, y, z;
  memcpy (&y, &a, FIXED_SIZE);
  x = 0;
  z = x - y;
  if (((x ^ y) >> I_F_BITS) & 1)
    {
      if (((z ^ x) >> I_F_BITS) & 1)
        {
          z = 1;
          z = z << I_F_BITS;
          if (x >= 0)
            z--;
        }
    }
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_SSNEG */

#if defined(FIXED_USNEG)
FIXED_C_TYPE
FIXED_USNEG (FIXED_C_TYPE a __attribute__ ((__unused__)))
{
  FIXED_C_TYPE c;
  INT_C_TYPE z;
  z = 0;
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_USNEG */

#if defined(FIXED_ASHLHELPER)
FIXED_C_TYPE
FIXED_ASHLHELPER (FIXED_C_TYPE a, word_type b, word_type satp)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, z;

#if defined (DINT_C_TYPE)
  DINT_C_TYPE dx, dz;
  memcpy (&x, &a, FIXED_SIZE);
  dx = (DINT_C_TYPE) x;
  if (b >= FIXED_WIDTH)
    dz = dx << FIXED_WIDTH;
  else
    dz = dx << b;
  if (satp)
    FIXED_SATURATE1 (&dz);
  z = (INT_C_TYPE) dz;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;

#else /* No DINT_C_TYPE */
  INT_C_TYPE r, s;
  memcpy (&x, &a, FIXED_SIZE);
  /* We need to shift left x by b bits to {r, s}.  */
  if (b >= FIXED_WIDTH)
    {
      r = b;
      s = 0;
    }
  else
    {
      s = x << b;
      r = x >> (FIXED_WIDTH - b);
    }
  if (satp)
    FIXED_SATURATE2 (&r, &s);
  z = s;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
#endif
}
#endif /* FIXED_ASHLHELPER */

#if defined(FIXED_ASHL)
FIXED_C_TYPE
FIXED_ASHL (FIXED_C_TYPE a, word_type b)
{
  return FIXED_ASHLHELPER (a, b, 0);
}
#endif /* FIXED_ASHL */

#if defined(FIXED_ASHR)
FIXED_C_TYPE
FIXED_ASHR (FIXED_C_TYPE a, word_type b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, z;
  memcpy (&x, &a, FIXED_SIZE);
  z = x >> b;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_ASHR */

#if defined(FIXED_LSHR)
FIXED_C_TYPE
FIXED_LSHR (FIXED_C_TYPE a, word_type b)
{
  FIXED_C_TYPE c;
  INT_C_TYPE x, z;
  memcpy (&x, &a, FIXED_SIZE);
  z = x >> b;
#if HAVE_PADDING_BITS
  z = z << PADDING_BITS;
  z = z >> PADDING_BITS;
#endif
  memcpy (&c, &z, FIXED_SIZE);
  return c;
}
#endif /* FIXED_LSHR */

#if defined(FIXED_SSASHL)
FIXED_C_TYPE
FIXED_SSASHL (FIXED_C_TYPE a, word_type b)
{
  return FIXED_ASHLHELPER (a, b, 1);
}
#endif /* FIXED_SSASHL */

#if defined(FIXED_USASHL)
FIXED_C_TYPE
FIXED_USASHL (FIXED_C_TYPE a, word_type b)
{
  return FIXED_ASHLHELPER (a, b, 1);
}
#endif /* FIXED_USASHL */

#if defined(FIXED_CMP)
word_type
FIXED_CMP (FIXED_C_TYPE a, FIXED_C_TYPE b)
{
  INT_C_TYPE x, y;
  memcpy (&x, &a, FIXED_SIZE);
  memcpy (&y, &b, FIXED_SIZE);

  if (x < y)
    return 0;
  else if (x > y)
    return 2;

  return 1;
}
#endif /* FIXED_CMP */

