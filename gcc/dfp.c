/* Decimal floating point support.
   Copyright (C) 2005 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2, or (at your option) any later
   version.

   GCC is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "toplev.h"
#include "real.h"
#include "tm_p.h"
#include "dfp.h"

/* The order of the following headers is important for making sure
   decNumber structure is large enough to hold decimal128 digits.  */

#include "decimal128.h"
#include "decimal64.h"
#include "decimal32.h"
#include "decNumber.h"

/* Useful for munging between formats.  */
static decNumber dn;
static decNumber dn2;
static decNumber dn3;
static decContext set;
static char string[256];

/* Initialize a REAL_VALUE_TYPE (decimal encoded) from a decNumber.
   Can utilize status passed in via decContext parameter, if some
   previous operation had interesting status.  */
static void
decimal_from_decnumber (REAL_VALUE_TYPE *r, decNumber *dn, decContext *set)
{
  memset (r, 0, sizeof (REAL_VALUE_TYPE));

  r->cl = rvc_normal;
  if (decNumberIsZero (dn))
    r->cl = rvc_zero;
  if (decNumberIsNaN (dn))
    r->cl = rvc_nan;
  if (decNumberIsInfinite (dn))
    r->cl = rvc_inf;
  if (set->status & DEC_Overflow)
    r->cl = rvc_inf;
  if (decNumberIsNegative (dn))
    r->sign = 1;
  r->decimal = 1;

  if (r->cl != rvc_normal)
    return;

  decContextDefault (set, DEC_INIT_DECIMAL128);
  set->traps = 0;

  decimal128FromNumber ((decimal128 *)r->sig, dn, set);
}

/* Create decimal encoded R from string S.  */
static void
decimal_from_string (REAL_VALUE_TYPE *r, const char *s)
{
  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  decNumberFromString (&dn, (char *)s, &set);

  /* It would be more efficient to store directly in decNumber format,
     but that is impractical from current data structure size.
     Encoding as a decimal128 is much more compact.  */
  decimal_from_decnumber (r, &dn, &set);
}

/* Wrapper to handle decimal strings too.  */
void
decimal_real_from_string (REAL_VALUE_TYPE *r, const char *s, 
			  enum machine_mode mode)
{
  if (!DECIMAL_FLOAT_MODE_P (mode))
    real_from_string (r, s);
  else
    decimal_from_string (r, s);
}

/* Initialize a decNumber from a GCC REAL_VALUE_TYPE.  */
static void
decimal_to_decnumber (const REAL_VALUE_TYPE *r, decNumber *dn)
{
  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;
  switch (r->cl)
    {
    case rvc_zero:
      decNumberZero (dn);
      break;
    case rvc_inf:
      decNumberFromString (dn, (char *)"Infinity", &set);
      break;
    case rvc_nan:
      if (r->signalling)
        decNumberFromString (dn, (char *)"snan", &set);
      else
        decNumberFromString (dn, (char *)"nan", &set);
      break;
    case rvc_normal:
      gcc_assert (r->decimal);
      decimal128ToNumber ((decimal128 *)r->sig, dn);
      break;
    default:
      gcc_unreachable ();
    }

  /* Fix up sign bit.  */
  if (r->sign != decNumberIsNegative (dn))
    decNumberNegate (dn);
}

void 
encode_decimal32 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		  long *buf, const REAL_VALUE_TYPE *r)
{
  decimal32 d32;

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  decimal_to_decnumber (r, &dn); 
  decimal32FromNumber (&d32, &dn, &set);

  if (FLOAT_WORDS_BIG_ENDIAN)
    buf[0] = *(uint32_t *) d32.bytes;
  else
    buf[0] = __dec_byte_swap (*(uint32_t *) d32.bytes);
}

void decode_decimal32 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		       REAL_VALUE_TYPE *r ATTRIBUTE_UNUSED,
		       const long *buf ATTRIBUTE_UNUSED)
{
  /* FIXME: Do something.  */
}


void 
encode_decimal64 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		  long *buf, const REAL_VALUE_TYPE *r)
{
  decimal64 d64;

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  decimal_to_decnumber (r, &dn);
  decimal64FromNumber (&d64, &dn, &set);

  if (FLOAT_WORDS_BIG_ENDIAN)
    {
      buf[0] = *(uint32_t *) &d64.bytes[0];
      buf[1] = *(uint32_t *) &d64.bytes[4];
    }
  else
    {
      buf[1] = __dec_byte_swap (*(uint32_t *) &d64.bytes[0]);
      buf[0] = __dec_byte_swap (*(uint32_t *) &d64.bytes[4]);
    }
}

void 
decode_decimal64 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		  REAL_VALUE_TYPE *r ATTRIBUTE_UNUSED,
		  const long *buf ATTRIBUTE_UNUSED)
{
  /* FIXME: Do something.  */
}

void 
encode_decimal128 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		   long *buf, const REAL_VALUE_TYPE *r)
{
  decimal128 d128;

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  decimal_to_decnumber (r, &dn);
  decimal128FromNumber (&d128, &dn, &set);

  if (FLOAT_WORDS_BIG_ENDIAN)
    {
      buf[0] = *(uint32_t *) &d128.bytes[0];
      buf[1] = *(uint32_t *) &d128.bytes[4];
      buf[2] = *(uint32_t *) &d128.bytes[8];
      buf[3] = *(uint32_t *) &d128.bytes[12];
    }
  else
    {
      buf[0] = __dec_byte_swap (*(uint32_t *) &d128.bytes[12]);
      buf[1] = __dec_byte_swap (*(uint32_t *) &d128.bytes[8]);
      buf[2] = __dec_byte_swap (*(uint32_t *) &d128.bytes[4]);
      buf[3] = __dec_byte_swap (*(uint32_t *) &d128.bytes[0]);
    }
}

void 
decode_decimal128 (const struct real_format *fmt ATTRIBUTE_UNUSED,
		   REAL_VALUE_TYPE *r ATTRIBUTE_UNUSED,
		   const long *buf ATTRIBUTE_UNUSED)
{
  /* FIXME: Do something.  */
}

/* Helper function to convert from a binary real internal
   representation.  */
static void
decimal_to_binary (REAL_VALUE_TYPE *to, const REAL_VALUE_TYPE *from,
		   enum machine_mode mode)
{
  decimal128 *d128;
  d128 = (decimal128 *)from->sig;

  decimal128ToString (d128, string);
  real_from_string3 (to, string, mode);
}


/* Helper function to convert from a binary real internal
   representation.  */
static void
decimal_from_binary (REAL_VALUE_TYPE *to, const REAL_VALUE_TYPE *from)
{
  /* We convert to string then convert to decNumber then to
     decimal128.  */
  real_to_decimal (string, from, sizeof (string), 0, 1);
  decimal_from_string (to, string);
}

/* Helper function to real.c::do_compare to handle decimal internal
   represenation including when one of the operands is still in the
   binary internal representation.  */
int
decimal_do_compare (const REAL_VALUE_TYPE *a, const REAL_VALUE_TYPE *b,
		    int nan_result)
{
  REAL_VALUE_TYPE a1;
  REAL_VALUE_TYPE b1;

  /* If either op is not a decimal, create a temporary decimal
     versions.  */
  if (!a->decimal)
    {
      decimal_from_binary (&a1, a);
      a = &a1;
    }
  if (!b->decimal)
    {
      decimal_from_binary (&b1, b);
      b = &b1;
    }
    
  /* Convert into decNumber form for comparison operation.  */
  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;  
  decimal128ToNumber ((decimal128 *) a->sig, &dn2);
  decimal128ToNumber ((decimal128 *) b->sig, &dn3);

  /* Finally, do the comparison.  */
  decNumberCompare (&dn, &dn2, &dn3, &set);

  /* Return the comparison result.  */
  if (decNumberIsNaN (&dn))
    return nan_result;
  else if (decNumberIsZero (&dn))
    return 0;
  else if (decNumberIsNegative (&dn))
    return -1;
  else 
    return 1;
}

/* Helper to round_for_format, handling decimal float types.  */
void
decimal_round_for_format (const struct real_format *fmt, REAL_VALUE_TYPE *r)
{

  /* Real encoding occurs later.  */
  if (r->cl != rvc_normal)
    return;

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;
  decimal128ToNumber ((decimal128 *) r->sig, &dn);

  if (fmt == &decimal_quad_format)
    {
      /* The internal format is already in this format.  */
      return;
    }
  else if (fmt == &decimal_single_format)
    {
      decimal32 d32;
      decContextDefault (&set, DEC_INIT_DECIMAL32);
      set.traps = 0;

      decimal32FromNumber (&d32, &dn, &set);
      decimal32ToNumber (&d32, &dn);
    }
  else if (fmt == &decimal_double_format)
    {
      decimal64 d64;
      decContextDefault (&set, DEC_INIT_DECIMAL64);
      set.traps = 0;

      decimal64FromNumber (&d64, &dn, &set);
      decimal64ToNumber (&d64, &dn);
    }
  else
    gcc_unreachable ();

  decimal_from_decnumber (r, &dn, &set);
}

/* Helper to real_convert, handling conversions between binary and
   decimal types.  */
void
decimal_real_convert (REAL_VALUE_TYPE *r, enum machine_mode mode, 
		      const REAL_VALUE_TYPE *a)
{
  const struct real_format *fmt = REAL_MODE_FORMAT (mode);

  if (a->decimal && fmt->b == 10)
    return;
  if (a->decimal)
      decimal_to_binary (r, a, mode);
  else
      decimal_from_binary (r, a);
}

/* Helper to print out internal representation of decimal floating types.  */

/* Render R as a decimal floating point constant.  Emit DIGITS
   significant digits in the result, bounded by BUF_SIZE.  If DIGITS
   is 0, choose the maximum for the representation.  If
   CROP_TRAILING_ZEROS, strip trailing zeros.  Currently, not honoring
   DIGITS or CROP_TRAILING_ZEROS.  */
void decimal_real_to_decimal (char *str, const REAL_VALUE_TYPE *r_orig,
			      size_t buf_size,
			      size_t digits ATTRIBUTE_UNUSED,
			      int crop_trailing_zeros ATTRIBUTE_UNUSED)
{
  decimal128 *d128 = (decimal128*) r_orig->sig;

  /* decimal128ToString requires space for at least 24 characters;
     Require two more for suffix.  */
  gcc_assert (buf_size >= 24);
  decimal128ToString (d128, str);
}

static bool
decimal_do_add (REAL_VALUE_TYPE *r, const REAL_VALUE_TYPE *op0,
		const REAL_VALUE_TYPE *op1, int subtract_p)
{
  decimal_to_decnumber (op0, &dn2);
  decimal_to_decnumber (op1, &dn3);

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  if (subtract_p)
    decNumberSubtract (&dn, &dn2, &dn3, &set);
  else 
    decNumberAdd (&dn, &dn2, &dn3, &set);

  decimal_from_decnumber (r, &dn, &set);

  /* Return true, if inexact.  */
  return (set.status & DEC_Inexact);
}

static bool
decimal_do_multiply (REAL_VALUE_TYPE *r, const REAL_VALUE_TYPE *op0,
		     const REAL_VALUE_TYPE *op1)
{
  decimal_to_decnumber (op0, &dn2);
  decimal_to_decnumber (op1, &dn3);

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  decNumberMultiply (&dn, &dn2, &dn3, &set);
  decimal_from_decnumber (r, &dn, &set);

  /* Return true, if inexact.  */
  return (set.status & DEC_Inexact);
}

static bool
decimal_do_divide (REAL_VALUE_TYPE *r, const REAL_VALUE_TYPE *op0,
		   const REAL_VALUE_TYPE *op1)
{
  decimal_to_decnumber (op0, &dn2);
  decimal_to_decnumber (op1, &dn3);

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;

  decNumberDivide (&dn, &dn2, &dn3, &set);
  decimal_from_decnumber (r, &dn, &set);

  /* Return true, if inexact.  */
  return (set.status & DEC_Inexact);
}

/* Helper to do_fix_trunc.  */
void
decimal_do_fix_trunc (REAL_VALUE_TYPE *r, const REAL_VALUE_TYPE *a)
{
  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;
  set.round = DEC_ROUND_DOWN;
  decimal128ToNumber ((decimal128 *) a->sig, &dn2);

  decNumberToIntegralValue (&dn, &dn2, &set);
  decimal_from_decnumber (r, &dn, &set);
}

/* Helper to real_to_integer.  */
HOST_WIDE_INT
decimal_real_to_integer (const REAL_VALUE_TYPE *r)
{
  REAL_VALUE_TYPE to;

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;
  set.round = DEC_ROUND_DOWN;
  decimal128ToNumber ((decimal128 *) r->sig, &dn);

  decNumberToIntegralValue (&dn2, &dn, &set);
  decNumberZero (&dn3);
  decNumberRescale (&dn, &dn2, &dn3, &set);

  /* Convert to REAL_VALUE_TYPE and call appropriate
     conversion function.  */
  decNumberToString (&dn, string);
  real_from_string (&to, string);
  return real_to_integer (&to);
}


void
decimal_real_to_integer2 (HOST_WIDE_INT *plow, HOST_WIDE_INT *phigh,
			  const REAL_VALUE_TYPE *r)
{
  REAL_VALUE_TYPE to;

  decContextDefault (&set, DEC_INIT_DECIMAL128);
  set.traps = 0;
  set.round = DEC_ROUND_DOWN;
  decimal128ToNumber ((decimal128 *) r->sig, &dn);

  decNumberToIntegralValue (&dn2, &dn, &set);
  decNumberZero (&dn3);
  decNumberRescale (&dn, &dn2, &dn3, &set);

  /* Conver to REAL_VALUE_TYPE and call appropriate conversion
     function.  */
  decNumberToString (&dn, string);
  real_from_string (&to, string);
  real_to_integer2 (plow, phigh, &to);
}

bool
decimal_real_arithmetic (REAL_VALUE_TYPE *r, int icode,
			 const REAL_VALUE_TYPE *op0,
			 const REAL_VALUE_TYPE *op1)
{
  enum tree_code code = icode;
  REAL_VALUE_TYPE a1;
  REAL_VALUE_TYPE b1;

  /* If either op is not a decimal, create a temporary decimal
     versions.  */
  if (!op0->decimal)
    {
      decimal_from_binary (&a1, op0);
      op0 = &a1;
    }
  if (op1 && !op1->decimal)
    {
      decimal_from_binary (&b1, op1);
      op1 = &b1;
    }

  switch (code)
    {
    case PLUS_EXPR:
      (void) decimal_do_add (r, op0, op1, 0);
      break;

    case MINUS_EXPR:
      (void) decimal_do_add (r, op0, op1, 1);
      break;

    case MULT_EXPR:
      (void) decimal_do_multiply (r, op0, op1);
      break;

    case RDIV_EXPR:
      (void) decimal_do_divide (r, op0, op1);
      break;

    case MIN_EXPR:
      if (op1->cl == rvc_nan)
        *r = *op1;
      else if (real_compare (UNLT_EXPR, op0, op1))
        *r = *op0;
      else
        *r = *op1;
      break;

    case MAX_EXPR:
      if (op1->cl == rvc_nan)
        *r = *op1;
      else if (real_compare (LT_EXPR, op0, op1))
        *r = *op1;
      else
        *r = *op0;
      break;

    case NEGATE_EXPR:
      {
	decimal128 *d128;
	*r = *op0;
	d128 = (decimal128 *) r->sig;
	d128->bytes[0] ^= 1 << 7;  /* Flip high bit */
	r->sign ^= 1; /* Keep in sync.  */
      }
      break;

    case ABS_EXPR:
      {
        decimal128 *d128;
        *r = *op0;
        d128 = (decimal128 *)r->sig;
        d128->bytes[0] &= 0x7f;  /* Clear high bit */
	r->sign = 0;
      }
      break;

    case FIX_TRUNC_EXPR:
      decimal_do_fix_trunc (r, op0);
      break;

    default:
      gcc_unreachable ();
    }

  /* FIXME: All operations are inexact for now due to unknown working
     precision.  */
  return true;
}

/* Fills R with the largest finite value representable in mode MODE.
   If SIGN is nonzero, R is set to the most negative finite value.  */

void
decimal_real_maxval (REAL_VALUE_TYPE *r, int sign, enum machine_mode mode)
{ 
  char *max;

  /* FIXME: These modes may not be defined on all platforms. */
  switch (mode)
    {
    case SDmode:
      max = (char *)"9.999999E96";
      break;
    case DDmode:
      max = (char *)"9.999999999999999E384";
      break;
    case TDmode:
      max = (char *)"9.999999999999999999999999999999999E6144";
      break;
    default:
      gcc_unreachable ();
    }

  decimal_from_string (r, max);
  if (sign)
    r->sig[0] |= 0x80000000;
}
