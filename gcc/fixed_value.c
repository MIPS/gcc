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
#include "fixed_value.h"
#include "real.h"

static void do_fixed_add (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			  const FIXED_VALUE_TYPE *, int);
static void do_fixed_multiply (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			       const FIXED_VALUE_TYPE *);
static void do_fixed_divide (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			     const FIXED_VALUE_TYPE *);
static void do_fixed_shift (FIXED_VALUE_TYPE *, const FIXED_VALUE_TYPE *,
			    const FIXED_VALUE_TYPE *, int);

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

/* Initialize from a decimal or hexadecimal string.  */

void
fixed_from_string (FIXED_VALUE_TYPE *f, const char *str, tree type)
{
  REAL_VALUE_TYPE real_value, fixed_value, base_value;
  char base_string[10];
  unsigned int fbit;

  f->mode = TYPE_MODE (type);
  fbit = TYPE_FBIT (type);

  sprintf (base_string, "0x1.0p%d", fbit);
  real_from_string (&real_value, str);
  real_from_string (&base_value, base_string);
  real_arithmetic (&fixed_value, MULT_EXPR, &real_value, &base_value);
  real_to_integer2 (&f->data.low, &f->data.high,  &fixed_value);
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
  char base_string[10];
  REAL_VALUE_TYPE real_value, base_value, fixed_value;

  sprintf (base_string, "0x1.0p%d", GET_MODE_FBIT (f_orig->mode));
  real_from_string (&base_value, base_string);
  real_from_integer (&real_value, VOIDmode, f_orig->data.low, f_orig->data.high,
		     UNSIGNED_FIXED_POINT_MODE_P (f_orig->mode));
  real_arithmetic (&fixed_value, RDIV_EXPR, &real_value, &base_value);
  real_to_decimal (str, &fixed_value, buf_size, 0, 1);
}

/* Calculate F = A + (SUBTRACT_P ? -B : B).  */

static void
do_fixed_add (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
	      const FIXED_VALUE_TYPE *b, int subtract_p)
{
  f->mode = a->mode;
  f->data = double_int_add (a->data,
			    subtract_p ? double_int_neg (b->data) : b->data);
  f->data = double_int_ext (f->data,
			    SIGNED_FIXED_POINT_MODE_P (f->mode)
			    + GET_MODE_FBIT (f->mode)
			    + GET_MODE_IBIT (f->mode),
			    UNSIGNED_FIXED_POINT_MODE_P (f->mode));
}

/* Calculate F = A * B.  */

static void
do_fixed_multiply (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
		   const FIXED_VALUE_TYPE *b)
{
  f->mode = a->mode;
  if (GET_MODE_PRECISION (f->mode) <= HOST_BITS_PER_WIDE_INT)
    {
      f->data = double_int_mul (a->data, b->data);
      lshift_double (f->data.low, f->data.high,
		     (-GET_MODE_FBIT (f->mode)),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &f->data.low, & f->data.high,
		     SIGNED_FIXED_POINT_MODE_P (f->mode));
      f->data = double_int_ext (f->data,
				SIGNED_FIXED_POINT_MODE_P (f->mode)
				+ GET_MODE_FBIT (f->mode)
				+ GET_MODE_IBIT (f->mode),
				UNSIGNED_FIXED_POINT_MODE_P (f->mode));
    }
  else
    {
      /* FIXME */
      printf ("ERROR\n");
    }
}

/* Calculate F = A / B.  */

static void
do_fixed_divide (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
		 const FIXED_VALUE_TYPE *b)
{
  f->mode = a->mode;
  if (GET_MODE_PRECISION (f->mode) <= HOST_BITS_PER_WIDE_INT)
    {
      lshift_double (a->data.low, a->data.high,
		     GET_MODE_FBIT (f->mode),
		     2 * HOST_BITS_PER_WIDE_INT,
		     &f->data.low, & f->data.high,
		     SIGNED_FIXED_POINT_MODE_P (f->mode));
      f->data = double_int_div (f->data, b->data,
				UNSIGNED_FIXED_POINT_MODE_P (f->mode),
				TRUNC_DIV_EXPR);
      f->data = double_int_ext (f->data,
				SIGNED_FIXED_POINT_MODE_P (f->mode)
				+ GET_MODE_FBIT (f->mode)
				+ GET_MODE_IBIT (f->mode),
				UNSIGNED_FIXED_POINT_MODE_P (f->mode));
    }
  else
    {
      /* FIXME */
      printf ("ERROR\n");
    }
}

/* Calculate F = A << B if LEFT_P.  Otherwies, F = A >> B.  */

static void
do_fixed_shift (FIXED_VALUE_TYPE *f, const FIXED_VALUE_TYPE *a,
	      const FIXED_VALUE_TYPE *b, int left_p)
{
  f->mode = a->mode;
  lshift_double (a->data.low, a->data.high,
		 left_p ? b->data.low : (-b->data.low),
		 2 * HOST_BITS_PER_WIDE_INT,
		 &f->data.low, & f->data.high,
		 SIGNED_FIXED_POINT_MODE_P (f->mode));
  f->data = double_int_ext (f->data,
			    SIGNED_FIXED_POINT_MODE_P (f->mode)
			    + GET_MODE_FBIT (f->mode)
			    + GET_MODE_IBIT (f->mode),
			    UNSIGNED_FIXED_POINT_MODE_P (f->mode));
}

/* Perform the binary or unary operation described by CODE.
   For a unary operation, leave OP1 NULL.  This function returns
   true if the result may be inexact due to loss of precision.  */

void
fixed_arithmetic (FIXED_VALUE_TYPE *f, int icode, const FIXED_VALUE_TYPE *op0,
		  const FIXED_VALUE_TYPE *op1)
{
  switch (icode)
    {
    case NEGATE_EXPR:
      f->mode = op0->mode;
      f->data = double_int_neg (op0->data);
      f->data = double_int_ext (f->data,
				SIGNED_FIXED_POINT_MODE_P (f->mode)
				+ GET_MODE_FBIT (f->mode)
				+ GET_MODE_IBIT (f->mode),
				UNSIGNED_FIXED_POINT_MODE_P (f->mode));
      break;

    case PLUS_EXPR:
      do_fixed_add (f, op0, op1, 0);
      break;

    case MINUS_EXPR:
      do_fixed_add (f, op0, op1, 1);
      break;

    case MULT_EXPR:
      do_fixed_multiply (f, op0, op1);
      break;

    case TRUNC_DIV_EXPR:
      do_fixed_divide (f, op0, op1);
      break;

    case LSHIFT_EXPR:
      do_fixed_shift (f, op0, op1, 1);
      break;

    case RSHIFT_EXPR:
      do_fixed_shift (f, op0, op1, 0);
      break;

    default:
      gcc_unreachable ();
    }
}

FIXED_VALUE_TYPE
fixed_arithmetic2 (int icode, const FIXED_VALUE_TYPE *op0,
		   const FIXED_VALUE_TYPE *op1)
{
  FIXED_VALUE_TYPE f;
  fixed_arithmetic (&f, icode, op0, op1);
  return f;
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
