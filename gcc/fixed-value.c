/* Fixed-point arithmetic support.
   Copyright (C) 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "toplev.h"
#include "fixed-value.h"

static int check_real_for_fixed_mode (REAL_VALUE_TYPE *, unsigned int);
static int get_fixed_sign_bit (double_int, int);
static bool do_fixed_add (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			  const FIXED_VALUE_TYPE *, int, int);
static bool do_fixed_multiply (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			       const FIXED_VALUE_TYPE *, int);
static bool do_fixed_divide (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			     const FIXED_VALUE_TYPE *, int);
static bool do_fixed_shift (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			    const FIXED_VALUE_TYPE *, int, int);
static bool do_fixed_neg (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *, int);
static bool fixed_saturate1 (unsigned int, double_int, double_int *, int);
static bool fixed_saturate2 (unsigned int, double_int, double_int,
			     double_int *, int);

/* Compare two fixed objects for bitwise identity.  */

bool
fixed_identical (const FIXED_VALUE_TYPE *a, const FIXED_VALUE_TYPE *b)
{
  return a->mode == b->mode && a->data.high == b->data.high
	 && a->data.low == b->data.low;
}

/* Calculate a hash value.  */

unsigned int
fixed_hash (const FIXED_VALUE_TYPE *f)
{
  return (unsigned int) (f->data.low ^ f->data.high);
}

/* Check REAL_VALUE against the range of the fixed-point mode.
   Return 0, if it is within the range.
          1, if it is less than the minimum.
          2, if it is greater than the maximum + 1.
          3, if it is equal to the maximum + 1.  */

int
check_real_for_fixed_mode (REAL_VALUE_TYPE *real_value, unsigned int mode)
{
  REAL_VALUE_TYPE max_value, min_value;
  char max_string[20], min_string[20];

  if (SIGNED_FIXED_POINT_MODE_P (mode))
    {
      sprintf (max_string, "0x1.0p%d", GET_MODE_IBIT (mode));
      sprintf (min_string, "-0x1.0p%d", GET_MODE_IBIT (mode));
      real_from_string (&max_value, max_string);
      real_from_string (&min_value, min_string);
    }
  else
    {
      sprintf (max_string, "0x1.0p%d", GET_MODE_IBIT (mode));
      real_from_string (&max_value, max_string);
      real_from_string (&min_value, "0.0");
    }

  if (real_compare (LT_EXPR, real_value, &min_value))
    return 1; 
  if (real_compare (GT_EXPR, real_value, &max_value))
    return 2; 
  if (real_compare (EQ_EXPR, real_value, &max_value))
    return 3; 
  return 0;
}

/* Initialize from a decimal or hexadecimal string.  */

void
fixed_from_string (FIXED_VALUE_TYPE *f, const char *str, tree type)
{
  REAL_VALUE_TYPE real_value, fixed_value, base_value;
  char base_string[20];
  unsigned int fbit;
  int temp;

  f->mode = TYPE_MODE (type);
  fbit = TYPE_FBIT (type);

  sprintf (base_string, "0x1.0p%d", fbit);
  real_from_string (&real_value, str);
  temp = check_real_for_fixed_mode (&real_value, f->mode);
  /* We don't want to warn the case when the _Fract value is 1.0.  */
  if (temp == 1 || temp == 2 || (temp == 3 && ALL_ACCUM_MODE_P (f->mode)))
    warning (OPT_Woverflow,
	     "large fixed-point constant implicitly truncated to fixed-point type");
  real_from_string (&base_value, base_string);
  real_arithmetic (&fixed_value, MULT_EXPR, &real_value, &base_value);
  real_to_integer2 (&f->data.low, &f->data.high,  &fixed_value);

  if (temp == 3 && ALL_FRACT_MODE_P (f->mode))
    {
      /* From the spec, we need to evaluate 1 to the maximal value.  */
      f->data.low = -1;
      f->data.high = -1;
      f->data = double_int_ext (f->data,
				GET_MODE_FBIT (f->mode)
				+ GET_MODE_IBIT (f->mode), 1);
    }
  else
    f->data = double_int_ext (f->data,
			      SIGNED_FIXED_POINT_MODE_P (f->mode)
			      + GET_MODE_FBIT (f->mode)
			      + GET_MODE_IBIT (f->mode),
			      UNSIGNED_FIXED_POINT_MODE_P (f->mode));
}

/* Render F as a decimal floating point constant.  */

void fixed_to_decimal (char *str, const FIXED_VALUE_TYPE *f_orig,
		       size_t buf_size)
{
  char base_string[20];
  REAL_VALUE_TYPE real_value, base_value, fixed_value;

  sprintf (base_string, "0x1.0p%d", GET_MODE_FBIT (f_orig->mode));
  real_from_string (&base_value, base_string);
  real_from_integer (&real_value, VOIDmode, f_orig->data.low, f_orig->data.high,
		     UNSIGNED_FIXED_POINT_MODE_P (f_orig->mode));
  real_arithmetic (&fixed_value, RDIV_EXPR, &real_value, &base_value);
  real_to_decimal (str, &fixed_value, buf_size, 0, 1);
}

/* If SATP, saturate A to the maximum or the minimum, and save to *F based on
   the machine mode MODE.
   This function assumes the width of double_int is greater than the width
   of the fixed-point value at the fixed-point mode.
   Return true, if !SATP and overflow.  */

static bool
fixed_saturate1 (unsigned int mode, double_int a, double_int *f, int satp)
{
  bool overflow = false;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (mode);
  int i_f_bits = GET_MODE_IBIT (mode) + GET_MODE_FBIT (mode);

  if (unsignedp) /* Unsigned type.  */
    {
      double_int max;
      max.low = -1;
      max.high = -1;
      max = double_int_ext (max, i_f_bits, 1);
      if (double_int_cmp (a, max, 1) == 1)
	{
	  if (satp)
	    *f = max;
	  else
	    overflow = true;
	}
    }
  else /* Signed type.  */
    {
      double_int max, min;
      max.high = -1;
      max.low = -1;
      max = double_int_ext (max, i_f_bits, 1);
      min.high = 0;
      min.low = 1;
      lshift_double (min.low, min.high, i_f_bits,
		     2 * HOST_BITS_PER_WIDE_INT,
		     &min.low, &min.high, 1);
      min = double_int_ext (min, 1 + i_f_bits, 0);
      if (double_int_cmp (a, max, 0) == 1)
	{
	  if (satp)
	    *f = max;
	  else
	    overflow = true;
	}
      else if (double_int_cmp (a, min, 0) == -1)
	{
	  if (satp)
	    *f = min;
	  else
	    overflow = true;
	}
    }
  return overflow;
}

/* If SATP, saturate {A_HIGH, A_LOW} to the maximum or the minimum, and
   save to *F based on the machine mode MODE.
   This function assumes the width of two double_int is greater than the width
   of the fixed-point value at the fixed-point mode.
   Return true, if !SATP and overflow.  */

static bool
fixed_saturate2 (unsigned int mode, double_int a_high, double_int a_low,
		 double_int *f, int satp)
{
  bool overflow = false;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (mode);
  int i_f_bits = GET_MODE_IBIT (mode) + GET_MODE_FBIT (mode);

  if (unsignedp) /* Unsigned type.  */
    {
      double_int max_r, max_s;
      max_r.high = 0;
      max_r.low = 0;
      max_s.high = -1;
      max_s.low = -1;
      max_s = double_int_ext (max_s, i_f_bits, 1);
      if (double_int_cmp (a_high, max_r, 1) == 1
	  || (double_int_equal_p (a_high, max_r) &&
	      double_int_cmp (a_low, max_s, 1) == 1))
	{
	  if (satp)
	    *f = max_s;
	  else
	    overflow = true;
	}
    }
  else /* Signed type.  */
    {
      double_int max_r, max_s, min_r, min_s;
      max_r.high = 0;
      max_r.low = 0;
      max_s.high = -1;
      max_s.low = -1;
      max_s = double_int_ext (max_s, i_f_bits, 1);
      min_r.high = -1;
      min_r.low = -1;
      min_s.high = 0;
      min_s.low = 1;
      lshift_double (min_s.low, min_s.high, i_f_bits,
		     2 * HOST_BITS_PER_WIDE_INT,
		     &min_s.low, &min_s.high, 1);
      min_s = double_int_ext (min_s, 1 + i_f_bits, 0);
      if (double_int_cmp (a_high, max_r, 0) == 1
	  || (double_int_equal_p (a_high, max_r) &&
	      double_int_cmp (a_low, max_s, 1) == 1))
	{
	  if (satp)
	    *f = max_s;
	  else
	    overflow = true;
	}
      else if (double_int_cmp (a_high, min_r, 0) == -1
	       || (double_int_equal_p (a_high, min_r) &&
		   double_int_cmp (a_low, min_s, 1) == -1))
	{
	  if (satp)
	    *f = min_s;
	  else
	    overflow = true;
	}
    }
  return overflow;
}

/* Return the sign bit based on I_F_BITS.  */

inline int get_fixed_sign_bit (double_int a, int i_f_bits)
{
  if (i_f_bits < HOST_BITS_PER_WIDE_INT)
    return (a.low >> i_f_bits) & 1;
  else
    return (a.high >> (i_f_bits - HOST_BITS_PER_WIDE_INT)) & 1;
}

/* Calculate F = A + (SUBTRACT_P ? -B : B).
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

static bool
do_fixed_add (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
	      const FIXED_VALUE_TYPE *b, int subtract_p, int satp)
{
  bool overflow = false;
  double_int temp = subtract_p ? double_int_neg (b->data) : b->data;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (a->mode);
  int i_f_bits = GET_MODE_IBIT (a->mode) + GET_MODE_FBIT (a->mode);
  f->mode = a->mode;
  f->data = double_int_add (a->data, temp);
  if (unsignedp) /* Unsigned type.  */
    {
      if (subtract_p) /* Unsigned subtraction.  */
	{
	  if (double_int_cmp (a->data, b->data, 1) == -1)
	    {
	      if (satp)
		{
		  f->data.high = 0;
		  f->data.low = 0;
		 }
	      else
		overflow = true;
	    }
	}
      else /* Unsigned addition.  */
	{
	  f->data = double_int_ext (f->data, i_f_bits, 1);
	  if (double_int_cmp (f->data, a->data, 1) == -1
	      || double_int_cmp (f->data, b->data, 1) == -1)
	    {
	      if (satp)
		{
		  f->data.high = -1;
		  f->data.low = -1;
		}
	      else
		overflow = true;
	    }
	}
    }
  else /* Signed type.  */
    {
      if (get_fixed_sign_bit (a->data, i_f_bits)
	  == get_fixed_sign_bit (temp, i_f_bits)
	  && get_fixed_sign_bit (a->data, i_f_bits)
	     != get_fixed_sign_bit (f->data, i_f_bits))
	{
	  if (satp)
	    {
	      f->data.low = 1;
	      f->data.high = 0;
	      lshift_double (f->data.low, f->data.high, i_f_bits,
			     2 * HOST_BITS_PER_WIDE_INT,
			     &f->data.low, &f->data.high, 1);
	      if (get_fixed_sign_bit (a->data, i_f_bits) == 0)
		{
		  double_int one;
		  one.low = 1;
		  one.high = 0;
		  f->data = double_int_add (f->data, double_int_neg (one));
		}
	    }
	  else
	    overflow = true;
	}
    }
  f->data = double_int_ext (f->data, (!unsignedp) + i_f_bits, unsignedp);
  return overflow;
}

/* Calculate F = A * B.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

static bool
do_fixed_multiply (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
		   const FIXED_VALUE_TYPE *b, int satp)
{
  bool overflow = false;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (a->mode);
  int i_f_bits = GET_MODE_IBIT (a->mode) + GET_MODE_FBIT (a->mode);
  f->mode = a->mode;
  if (GET_MODE_PRECISION (f->mode) <= HOST_BITS_PER_WIDE_INT)
    {
      f->data = double_int_mul (a->data, b->data);
      lshift_double (f->data.low, f->data.high,
		     (-GET_MODE_FBIT (f->mode)),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &f->data.low, &f->data.high, !unsignedp);
      overflow = fixed_saturate1 (f->mode, f->data, &f->data, satp);
    }
  else
    {
      /* The result of multiplication expands to two double_int.  */
      double_int a_high, a_low, b_high, b_low;
      double_int high_high, high_low, low_high, low_low;
      double_int r, s, temp1, temp2;
      int carry = 0;

      /* Decompose a and b to four double_int.  */
      a_high.low = a->data.high;
      a_high.high = 0;
      a_low.low = a->data.low;
      a_low.high = 0;
      b_high.low = b->data.high;
      b_high.high = 0;
      b_low.low = b->data.low;
      b_low.high = 0;

      /* Perform four multiplications.  */
      low_low = double_int_mul (a_low, b_low);
      low_high = double_int_mul (a_low, b_high);
      high_low = double_int_mul (a_high, b_low);
      high_high = double_int_mul (a_high, b_high);

      /* Accumulate four results to {r, s}.  */
      temp1.high = high_low.low;
      temp1.low = 0;
      s = double_int_add (low_low, temp1);
      if (double_int_cmp (s, low_low, 1) == -1
	  || double_int_cmp (s, temp1, 1) == -1)
	carry ++; /* Carry */
      temp1.high = s.high;
      temp1.low = s.low;
      temp2.high = low_high.low;
      temp2.low = 0;
      s = double_int_add (temp1, temp2);
      if (double_int_cmp (s, temp1, 1) == -1
	  || double_int_cmp (s, temp2, 1) == -1)
	carry ++; /* Carry */

      temp1.low = high_low.high;
      temp1.high = 0;
      r = double_int_add (high_high, temp1);
      temp1.low = low_high.high;
      temp1.high = 0;
      r = double_int_add (r, temp1);
      temp1.low = carry;
      temp1.high = 0;
      r = double_int_add (r, temp1);

      /* We need to add neg(b) to r, if a < 0.  */
      if (!unsignedp && a->data.high < 0)
	r = double_int_add (r, double_int_neg (b->data));
      /* We need to add neg(a) to r, if b < 0.  */
      if (!unsignedp && b->data.high < 0)
	r = double_int_add (r, double_int_neg (a->data));

      /* Shift right the result by FBIT.  */
      if (GET_MODE_FBIT (f->mode) == 2 * HOST_BITS_PER_WIDE_INT)
	{
	  s.low = r.low;
	  s.high = r.high;
	  if (unsignedp)
	    {
	      r.low = 0;
	      r.high = 0;
	    }
	  else
	    {
	      r.low = -1;
	      r.high = -1;
	    }
	  f->data.low = s.low;
	  f->data.high = s.high;
	}
      else
	{
	  lshift_double (s.low, s.high,
			 (-GET_MODE_FBIT (f->mode)),
			 2 * HOST_BITS_PER_WIDE_INT,
			 &s.low, &s.high, 0);
	  lshift_double (r.low, r.high,
			 (2 * HOST_BITS_PER_WIDE_INT - GET_MODE_FBIT (f->mode)),
			 2 * HOST_BITS_PER_WIDE_INT,
			 &f->data.low, &f->data.high, 0);
	  f->data.low = f->data.low | s.low;
	  f->data.high = f->data.high | s.high;
	  s.low = f->data.low;
	  s.high = f->data.high;
	  lshift_double (r.low, r.high,
			 (-GET_MODE_FBIT (f->mode)),
			 2 * HOST_BITS_PER_WIDE_INT,
			 &r.low, &r.high, !unsignedp);
	}

      overflow = fixed_saturate2 (f->mode, r, s, &f->data, satp);
    }

  f->data = double_int_ext (f->data, (!unsignedp) + i_f_bits, unsignedp);
  return false;
}

/* Calculate F = A / B.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

static bool
do_fixed_divide (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
		 const FIXED_VALUE_TYPE *b, int satp)
{
  bool overflow = false;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (a->mode);
  int i_f_bits = GET_MODE_IBIT (a->mode) + GET_MODE_FBIT (a->mode);
  f->mode = a->mode;
  if (GET_MODE_PRECISION (f->mode) <= HOST_BITS_PER_WIDE_INT)
    {
      lshift_double (a->data.low, a->data.high,
		     GET_MODE_FBIT (f->mode),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &f->data.low, &f->data.high, !unsignedp);
      f->data = double_int_div (f->data, b->data, unsignedp, TRUNC_DIV_EXPR);
      overflow = fixed_saturate1 (f->mode, f->data, &f->data, satp);
    }
  else
    {
      double_int pos_a, pos_b, r, s;
      double_int quo_r, quo_s, mod, temp;
      int num_of_neg = 0;
      int i;

      /* If a < 0, negate a.  */
      if (!unsignedp && a->data.high < 0)
	{
	  pos_a = double_int_neg (a->data);
	  num_of_neg ++;
	}
      else
	pos_a = a->data;

      /* If b < 0, negate b.  */
      if (!unsignedp && b->data.high < 0)
	{
	  pos_b = double_int_neg (b->data);
	  num_of_neg ++;
	}
      else
	pos_b = b->data;

      /* Left shift pos_a to {r, s} by FBIT.  */
      lshift_double (pos_a.low, pos_a.high,
		     GET_MODE_FBIT (f->mode),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &s.low, &s.high, 0);
      lshift_double (pos_a.low, pos_a.high,
		     - (2 * HOST_BITS_PER_WIDE_INT - GET_MODE_FBIT (f->mode)),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &r.low, &r.high, 0);

      /* Divide r by pos_b to quo_r.  The remanider is in mod.  */
      div_and_round_double (TRUNC_DIV_EXPR, 1, r.low, r.high, pos_b.low,
			    pos_b.high, &quo_r.low, &quo_r.high, &mod.low,
			    &mod.high);

      quo_s.high = 0;
      quo_s.low = 0;

      for (i = 0; i < 2 * HOST_BITS_PER_WIDE_INT; i++)
	{
	  /* Record the leftmost bit of mod.  */
	  int leftmost_mod = (mod.high < 0);

	  /* Shift left mod by 1 bit.  */
	  lshift_double (mod.low, mod.high, 1, 2 * HOST_BITS_PER_WIDE_INT,
			 &mod.low, &mod.high, 0);

	  /* Test the leftmost bit of s to add to mod.  */
	  if (s.high < 0)
	    mod.low += 1;

	  /* Shift left quo_s by 1 bit.  */
	  lshift_double (quo_s.low, quo_s.high, 1, 2 * HOST_BITS_PER_WIDE_INT,
			 &quo_s.low, &quo_s.high, 0);

	  /* Try to calculate (mod - pos_b).  */
	  temp = double_int_add (mod, double_int_neg (pos_b));

	  if (leftmost_mod == 1 || double_int_cmp (mod, pos_b, 1) != -1)
	    {
	      quo_s.low += 1;
	      mod = temp;
	    }

	  /* Shift left s by 1 bit.  */
	  lshift_double (s.low, s.high, 1, 2 * HOST_BITS_PER_WIDE_INT,
			 &s.low, &s.high, 0);

	}

      if (num_of_neg == 1)
	{
	  quo_s = double_int_neg (quo_s);
	  if (quo_s.high == 0 && quo_s.low == 0)
	    quo_r = double_int_neg (quo_r);
	  else
	    {
	      quo_r.low = ~quo_r.low;
	      quo_r.high = ~quo_r.high;
	    }
	}

      f->data = quo_s;
      overflow = fixed_saturate2 (f->mode, quo_r, quo_s, &f->data, satp);
    }

  f->data = double_int_ext (f->data, (!unsignedp) + i_f_bits, unsignedp);
  return overflow;
}

/* Calculate F = A << B if LEFT_P.  Otherwies, F = A >> B.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

static bool
do_fixed_shift (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
	      const FIXED_VALUE_TYPE *b, int left_p, int satp)
{
  bool overflow = false;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (a->mode);
  int i_f_bits = GET_MODE_IBIT (a->mode) + GET_MODE_FBIT (a->mode);
  f->mode = a->mode;

  if (b->data.low == 0)
    {
      f->data = a->data;
      return overflow;
    }

  if (GET_MODE_PRECISION (f->mode) <= HOST_BITS_PER_WIDE_INT || (!left_p))
    {
      lshift_double (a->data.low, a->data.high,
		     left_p ? b->data.low : (-b->data.low),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &f->data.low, &f->data.high, !unsignedp);
      if (left_p) /* Only left shift saturates.  */
	overflow = fixed_saturate1 (f->mode, f->data, &f->data, satp);
    }
  else /* We need two double_int to store the left-shift result.  */
    {
      double_int temp_high, temp_low;
      lshift_double (a->data.low, a->data.high,
		     b->data.low,
		     2 * HOST_BITS_PER_WIDE_INT,
		     &temp_low.low, &temp_low.high, !unsignedp);
      /* Logical shift right to temp_high.  */
      lshift_double (a->data.low, a->data.high,
		     b->data.low - 2 * HOST_BITS_PER_WIDE_INT,
		     2 * HOST_BITS_PER_WIDE_INT,
		     &temp_high.low, &temp_high.high, 0);
      if (!unsignedp && a->data.high < 0) /* Signed-extend temp_high.  */
	temp_high = double_int_ext (temp_high, b->data.low, unsignedp);
      f->data = temp_low;
      overflow = fixed_saturate2 (f->mode, temp_high, temp_low, &f->data, satp);
    }
  f->data = double_int_ext (f->data, (!unsignedp) + i_f_bits, unsignedp);
  return overflow;
}

/* Calculate F = -A.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

static bool
do_fixed_neg (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a, int satp)
{
  bool overflow = false;
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (a->mode);
  int i_f_bits = GET_MODE_IBIT (a->mode) + GET_MODE_FBIT (a->mode);
  f->mode = a->mode;
  f->data = double_int_neg (a->data);
  f->data = double_int_ext (f->data, (!unsignedp) + i_f_bits, unsignedp);

  if (unsignedp) /* Unsigned type.  */
    {
      if (f->data.low != 0 || f->data.high != 0)
	{
	  if (satp)
	    {
	      f->data.low = 0;
	      f->data.high = 0;
	    }
	  else
	    overflow = true;
	}
    }
  else /* Signed type.  */
    {
      if (!(f->data.high == 0 && f->data.low == 0)
	  && f->data.high == a->data.high && f->data.low == a->data.low )
	{ 
	  if (satp)
	    {
	      /* Saturate to the maximum by subtracting f->data by one.  */
	      f->data.low = -1;
	      f->data.high = -1;
	      f->data = double_int_ext (f->data, i_f_bits, 1);
	    }
	  else
	    overflow = true;
	}
    }
  return overflow;
}

/* Perform the binary or unary operation described by CODE.
   For a unary operation, leave OP1 NULL.
   Return true, if !SATP and overflow.  */

bool
fixed_arithmetic (FIXED_VALUE_TYPE *f, int icode, const FIXED_VALUE_TYPE *op0,
		  const FIXED_VALUE_TYPE *op1, int satp)
{
  switch (icode)
    {
    case NEGATE_EXPR:
      return do_fixed_neg (f, op0, satp);
      break;

    case PLUS_EXPR:
      return do_fixed_add (f, op0, op1, 0, satp);
      break;

    case MINUS_EXPR:
      return do_fixed_add (f, op0, op1, 1, satp);
      break;

    case MULT_EXPR:
      return do_fixed_multiply (f, op0, op1, satp);
      break;

    case TRUNC_DIV_EXPR:
      return do_fixed_divide (f, op0, op1, satp);
      break;

    case LSHIFT_EXPR:
      return do_fixed_shift (f, op0, op1, 1, satp);
      break;

    case RSHIFT_EXPR:
      return do_fixed_shift (f, op0, op1, 0, satp);
      break;

    default:
      gcc_unreachable ();
    }
  return false;
}

/* Compare fixed-point values by tree_code.  */

bool
fixed_compare (int icode, const FIXED_VALUE_TYPE *op0,
	       const FIXED_VALUE_TYPE *op1)
{
  enum tree_code code = icode;

  switch (code)
    {
    case NE_EXPR:
      return op0->mode != op1->mode
	     || !double_int_equal_p (op0->data, op1->data);

    case EQ_EXPR:
      return op0->mode == op1->mode
	     && double_int_equal_p (op0->data, op1->data);

    case LT_EXPR:
      return op0->mode == op1->mode
	     && double_int_cmp (op0->data, op1->data,
				UNSIGNED_FIXED_POINT_MODE_P (op0->mode)) == -1;

    case LE_EXPR:
      return op0->mode == op1->mode
	     && double_int_cmp (op0->data, op1->data,
				UNSIGNED_FIXED_POINT_MODE_P (op0->mode)) != 1;

    case GT_EXPR:
      return op0->mode == op1->mode
	     && double_int_cmp (op0->data, op1->data,
				UNSIGNED_FIXED_POINT_MODE_P (op0->mode)) == 1;

    case GE_EXPR:
      return op0->mode == op1->mode
	     && double_int_cmp (op0->data, op1->data,
				UNSIGNED_FIXED_POINT_MODE_P (op0->mode)) != -1;

    default:
      gcc_unreachable ();
    }
}

/* Extend or truncate to a new mode.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

bool
fixed_convert (FIXED_VALUE_TYPE *f, enum machine_mode mode,
               const FIXED_VALUE_TYPE *a, int satp)
{
  bool overflow = false;
  if (mode == a->mode)
    {
      *f = *a;
      return overflow;
    }
    
  if (GET_MODE_FBIT (mode) > GET_MODE_FBIT (a->mode))
    {
      /* Left shift a to temp_high, temp_low based on a->mode.  */
      double_int temp_high, temp_low;
      int amount = GET_MODE_FBIT (mode) - GET_MODE_FBIT (a->mode);
      lshift_double (a->data.low, a->data.high,
		     amount,
		     2 * HOST_BITS_PER_WIDE_INT,
		     &temp_low.low, &temp_low.high,
		     SIGNED_FIXED_POINT_MODE_P (a->mode));
      /* Logical shift right to temp_high.  */
      lshift_double (a->data.low, a->data.high,
		     amount - 2 * HOST_BITS_PER_WIDE_INT,
		     2 * HOST_BITS_PER_WIDE_INT,
		     &temp_high.low, &temp_high.high, 0);
      if (SIGNED_FIXED_POINT_MODE_P (a->mode)
	  && a->data.high < 0) /* Signed-extend temp_high.  */
	temp_high = double_int_ext (temp_high, amount, 0);
      f->mode = mode;
      f->data = temp_low;
      if (SIGNED_FIXED_POINT_MODE_P (a->mode) ==
	  SIGNED_FIXED_POINT_MODE_P (f->mode))
	overflow = fixed_saturate2 (f->mode, temp_high, temp_low, &f->data,
				    satp);
      else
	{
	  /* Take care of the cases when converting between signed and
	     unsigned.  */
	  if (SIGNED_FIXED_POINT_MODE_P (a->mode))
	    {
	      /* Signed -> Unsigned.  */
	      if (a->data.high < 0)
		{
		  if (satp)
		    {
		      f->data.low = 0;  /* Set to zero.  */
		      f->data.high = 0;  /* Set to zero.  */
		    }
		  else
		    overflow = true;
		}
	      else	
		overflow = fixed_saturate2 (f->mode, temp_high, temp_low,
					    &f->data, satp);
	    }
	  else
	    {
	      /* Unsigned -> Signed.  */
	      if (temp_high.high < 0)
		{
		  if (satp)
		    {
		      /* Set to maximum.  */
		      f->data.low = -1;  /* Set to all ones.  */
		      f->data.high = -1;  /* Set to all ones.  */
		      f->data = double_int_ext (f->data,
						GET_MODE_FBIT (f->mode) +
						GET_MODE_IBIT (f->mode),
						1); /* Clear the sign.  */
		    }
		  else
		    overflow = true;
		}
	      else
		overflow = fixed_saturate2 (f->mode, temp_high, temp_low,
					    &f->data, satp);
	    }
	}
    }
  else
    {
      /* Right shift a to temp based on a->mode.  */
      double_int temp;
      lshift_double (a->data.low, a->data.high,
		     GET_MODE_FBIT (mode) - GET_MODE_FBIT (a->mode),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &temp.low, &temp.high,
		     SIGNED_FIXED_POINT_MODE_P (a->mode));
      f->mode = mode;
      f->data = temp;
      if (SIGNED_FIXED_POINT_MODE_P (a->mode) ==
	  SIGNED_FIXED_POINT_MODE_P (f->mode))
	overflow = fixed_saturate1 (f->mode, f->data, &f->data, satp);
      else
	{
	  /* Take care of the cases when converting between signed and
	     unsigned.  */
	  if (SIGNED_FIXED_POINT_MODE_P (a->mode))
	    {
	      /* Signed -> Unsigned.  */
	      if (a->data.high < 0)
		{
		  if (satp)
		    {
		      f->data.low = 0;  /* Set to zero.  */
		      f->data.high = 0;  /* Set to zero.  */
		    }
		  else
		    overflow = true;
		}
	      else
		overflow = fixed_saturate1 (f->mode, f->data, &f->data, satp);
	    }
	  else
	    {
	      /* Unsigned -> Signed.  */
	      if (temp.high < 0)
		{
		  if (satp)
		    {
		      /* Set to maximum.  */
		      f->data.low = -1;  /* Set to all ones.  */
		      f->data.high = -1;  /* Set to all ones.  */
		      f->data = double_int_ext (f->data,
						GET_MODE_FBIT (f->mode) +
						GET_MODE_IBIT (f->mode),
						1); /* Clear the sign.  */
		    }
		  else
		    overflow = true;
		}
	      else
		overflow = fixed_saturate1 (f->mode, f->data, &f->data, satp);
	    }
	}
    }

  f->data = double_int_ext (f->data,
			    SIGNED_FIXED_POINT_MODE_P (f->mode)
			    + GET_MODE_FBIT (f->mode)
			    + GET_MODE_IBIT (f->mode),
			    UNSIGNED_FIXED_POINT_MODE_P (f->mode));
  return overflow;
}

/* Convert to a new fixed-point mode from an integer.
   If UNSIGNEDP, this integer is unsigned.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

bool
fixed_convert_from_int (FIXED_VALUE_TYPE *f, enum machine_mode mode,
			double_int a, int unsignedp, int satp)
{
  bool overflow = false;
  /* Left shift a to temp_high, temp_low.  */
  double_int temp_high, temp_low;
  int amount = GET_MODE_FBIT (mode);
  lshift_double (a.low, a.high,
		 amount,
		 2 * HOST_BITS_PER_WIDE_INT,
		 &temp_low.low, &temp_low.high, 0);

  /* Logical shift right to temp_high.  */
  lshift_double (a.low, a.high,
		 amount - 2 * HOST_BITS_PER_WIDE_INT,
		 2 * HOST_BITS_PER_WIDE_INT,
		 &temp_high.low, &temp_high.high, 0);
  if (!unsignedp && a.high < 0) /* Signed-extend temp_high.  */
    temp_high = double_int_ext (temp_high, amount, 0);

  f->mode = mode;
  f->data = temp_low;

  if (unsignedp == UNSIGNED_FIXED_POINT_MODE_P (f->mode))
    overflow = fixed_saturate2 (f->mode, temp_high, temp_low, &f->data, satp);
  else
    {
      /* Take care of the cases when converting between signed and unsigned.  */
      if (!unsignedp)
	{
	  /* Signed -> Unsigned.  */
	  if (a.high < 0)
	    {
	      if (satp)
		{
		  f->data.low = 0;  /* Set to zero.  */
		  f->data.high = 0;  /* Set to zero.  */
		}
	      else
		overflow = true;
	    }
	  else
	    overflow = fixed_saturate2 (f->mode, temp_high, temp_low, &f->data,
					satp);
	}
      else
	{
	  /* Unsigned -> Signed.  */
	  if (temp_high.high < 0)
	    {
	      if (satp)
		{
		  /* Set to maximum.  */
		  f->data.low = -1;  /* Set to all ones.  */
		  f->data.high = -1;  /* Set to all ones.  */
		  f->data = double_int_ext (f->data,
					    GET_MODE_FBIT (f->mode) +
					    GET_MODE_IBIT (f->mode),
					    1); /* Clear the sign.  */
		}
	      else
		overflow = true;
	    }
	  else
	    overflow = fixed_saturate2 (f->mode, temp_high, temp_low,
					&f->data, satp);
	}
    }
  f->data = double_int_ext (f->data,
			    SIGNED_FIXED_POINT_MODE_P (f->mode)
			    + GET_MODE_FBIT (f->mode)
			    + GET_MODE_IBIT (f->mode),
			    UNSIGNED_FIXED_POINT_MODE_P (f->mode));
  return overflow;
}

/* Convert to a new fixed-point mode from a real.
   If SATP, saturate the result to the max or the min.
   Return true, if !SATP and overflow.  */

bool
fixed_convert_from_real (FIXED_VALUE_TYPE *f, enum machine_mode mode,
			 const REAL_VALUE_TYPE *a, int satp)
{
  bool overflow = false;
  REAL_VALUE_TYPE real_value, fixed_value, base_value;
  char base_string[20];
  int unsignedp = UNSIGNED_FIXED_POINT_MODE_P (mode);
  int i_f_bits = GET_MODE_IBIT (mode) + GET_MODE_FBIT (mode);
  unsigned int fbit = GET_MODE_FBIT (mode);
  int temp;

  real_value = *a;
  f->mode = mode;
  sprintf (base_string, "0x1.0p%d", fbit);
  real_from_string (&base_value, base_string);
  real_arithmetic (&fixed_value, MULT_EXPR, &real_value, &base_value);
  real_to_integer2 (&f->data.low, &f->data.high,  &fixed_value);
  temp = check_real_for_fixed_mode (&real_value, mode);
  if (temp == 1) /* Minimum.  */
    {
      if (satp)
	{
	  if (unsignedp)
	    {
	      f->data.low = 0;
	      f->data.high = 0;
	    }
	  else
	    {
	      f->data.low = 1;
	      f->data.high = 0;
	      lshift_double (f->data.low, f->data.high, i_f_bits,
			     2 * HOST_BITS_PER_WIDE_INT,
			     &f->data.low, &f->data.high, 1);
	      f->data = double_int_ext (f->data, 1 + i_f_bits, 0);
	    }
	}
      else
	overflow = true;
    }
  else if (temp == 2 || temp == 3) /* Maximum.  */
    {
      if (satp)
	{
	  f->data.low = -1;
	  f->data.high = -1;
	  f->data = double_int_ext (f->data, i_f_bits, 1);
	}
      else
	overflow = true;
    }
  f->data = double_int_ext (f->data, (!unsignedp) + i_f_bits, unsignedp);
  return overflow;
}

/* Convert to a new real mode from a fixed-point.  */

void
real_convert_from_fixed (REAL_VALUE_TYPE *r, enum machine_mode mode,
			 const FIXED_VALUE_TYPE *f)
{
  char base_string[20];
  REAL_VALUE_TYPE base_value, fixed_value, real_value;

  sprintf (base_string, "0x1.0p%d", GET_MODE_FBIT (f->mode));
  real_from_string (&base_value, base_string);
  real_from_integer (&fixed_value, VOIDmode, f->data.low, f->data.high,
		     UNSIGNED_FIXED_POINT_MODE_P (f->mode));
  real_arithmetic (&real_value, RDIV_EXPR, &fixed_value, &base_value);
  real_convert (r, mode, &real_value);
}
