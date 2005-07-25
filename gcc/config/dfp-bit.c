/* This is a software decimal floating point library.
   Copyright (C) 2005 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

/* This implements IEEE 754R decimal floating point arithmetic, but
   does not provide a mechanism for setting the rounding mode, or for
   generating or handling exceptions.

   Contributed by Ben Elliston  <bje@au.ibm.com>.  */

/* The intended way to use this file is to make two copies, add `#define '
   to one copy, then compile both copies and add them to libgcc.a.  */

#include <string.h>

#include "tconfig.h"
#include "coretypes.h"
#include "tm.h"
#include "config/dfp-bit.h"

/* A pointer to a unary decNumber operation.  */
typedef decNumber* (*dfp_unary_func)
     (decNumber *, decNumber *, decContext *);

/* A pointer to a binary decNumber operation.  */
typedef decNumber* (*dfp_binary_func)
     (decNumber *, decNumber *, decNumber *, decContext *);


/* Unary operations.  */

static inline DFP_C_TYPE
dfp_unary_op (dfp_unary_func op, DFP_C_TYPE arg)
{
  DFP_C_TYPE result;
  decContext context;
  decNumber arg1, res;
  DFP_TYPE a, encoded_result;

  memcpy (&a, &arg, sizeof (a));

  decContextDefault (&context, DEC_INIT_BASE);
  context.digits = 5; /* FIXME: DECNUMDIGITS */
  /* Quiet NaNs for C.  */
  context.traps = 0;

  TO_INTERNAL (&a, &arg1);

  /* Perform the operation.  */
  op (&res, &arg1, &context);

  TO_ENCODED (&encoded_result, &res, &context);
  memcpy (&result, &encoded_result, sizeof (result));
  return result;
}

/* Binary operations.  */

static inline DFP_C_TYPE
dfp_binary_op (dfp_binary_func op, DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  DFP_C_TYPE result;
  decContext context;
  decNumber arg1, arg2, res;
  DFP_TYPE a, b, encoded_result;

  memcpy (&a, &arg_a, sizeof (a));
  memcpy (&b, &arg_b, sizeof (b));

  decContextDefault (&context, DEC_INIT_BASE);
  context.digits = 5; /* FIXME: DECNUMDIGITS */
  /* Quiet NaNs for C.  */
  context.traps = 0;

  TO_INTERNAL (&a, &arg1);
  TO_INTERNAL (&b, &arg2);

  /* Perform the operation.  */
  op (&res, &arg1, &arg2, &context);

  TO_ENCODED (&encoded_result, &res, &context);
  memcpy (&result, &encoded_result, sizeof (result));
  return result;
}

/* Comparison operations.  */

static inline int
dfp_compare_op (dfp_binary_func op, DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  DFP_TYPE a, b;
  decContext context;
  decNumber arg1, arg2, res;
  int result;

  memcpy (&a, &arg_a, sizeof (a));
  memcpy (&b, &arg_b, sizeof (b));

  decContextDefault (&context, DEC_INIT_BASE);
  context.digits = 20; /* FIXME: DECNUMDIGITS */

  /* Quiet NaNs for C.  */
  context.traps = 0;

  TO_INTERNAL (&a, &arg1);
  TO_INTERNAL (&b, &arg2);

  /* Perform the comparison.  */
  op (&res, &arg1, &arg2, &context);

  if (decNumberIsNegative (&res))
    result = -1;
  else if (decNumberIsZero (&res))
    result = 0;
  else
    result = 1;

  return result;
}


#if defined(L_addsub_sd) || defined(L_addsub_dd) || defined(L_addsub_td)
DFP_C_TYPE
DFP_ADD (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  return dfp_binary_op (decNumberAdd, arg_a, arg_b);
}

DFP_C_TYPE
DFP_SUB (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  return dfp_binary_op (decNumberSubtract, arg_a, arg_b);
}
#endif /* L_addsub */

#if defined(L_mul_sd) || defined(L_mul_dd) || defined(L_mul_td)
DFP_C_TYPE
DFP_MULTIPLY (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  return dfp_binary_op (decNumberMultiply, arg_a, arg_b);
}
#endif /* L_mul */

#if defined(L_div_sd) || defined(L_div_dd) || defined(L_div_td)
DFP_C_TYPE
DFP_DIVIDE (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  return dfp_binary_op (decNumberDivide, arg_a, arg_b);
}
#endif /* L_div */

#if 0

#if defined(L_plus_sd) || defined(L_plus_dd) || defined(L_plus_td)
DFP_C_TYPE
DFP_PLUS (DFP_C_TYPE arg)
{
  return dfp_unary_op (decNumberPlus, arg);
}
#endif /* L_plus */

#if defined(L_minus_sd) || defined(L_minus_dd) || defined(L_minus_td)
DFP_C_TYPE
DFP_MINUS (DFP_C_TYPE arg)
{
  return dfp_unary_op (decNumberMinus, arg);
}
#endif /* L_minus */

#endif /* 0 */

#if defined (L_eq_sd) || defined (L_eq_dd) || defined (L_eq_td)
int
DFP_EQ (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  int stat;
  stat = dfp_compare_op (decNumberCompare, arg_a, arg_b);
  /* For EQ return zero for true, nonzero for false.  */
  return stat != 0;
}
#endif /* L_eq */

#if defined (L_ne_sd) || defined (L_ne_dd) || defined (L_ne_td)
int
DFP_NE (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  int stat;
  stat = dfp_compare_op (decNumberCompare, arg_a, arg_b);
  /* For NE return nonzero for true, zero for false.  */
  return stat != 0;
}
#endif /* L_ne */

#if defined (L_lt_sd) || defined (L_lt_dd) || defined (L_lt_td)
int
DFP_LT (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  int stat;
  stat = dfp_compare_op (decNumberCompare, arg_a, arg_b);
  /* For LT return -1 (<0) for true, 1 for false.  */
  return (stat == -1) ? -1 : 1;
}
#endif /* L_lt */

#if defined (L_gt_sd) || defined (L_gt_dd) || defined (L_gt_td)
int
DFP_GT (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  int stat;
  stat = dfp_compare_op (decNumberCompare, arg_a, arg_b);
  /* For GT return 1 (>0) for true, -1 for false.  */
  return (stat == 1) ? 1 : -1;
}
#endif

#if defined (L_le_sd) || defined (L_le_dd) || defined (L_le_td)
int
DFP_LE (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  int stat;
  stat = dfp_compare_op (decNumberCompare, arg_a, arg_b);
  /* For LE return 0 (<= 0) for true, 1 for false.  */
  return stat == 1;
}
#endif /* L_le */

#if defined (L_ge_sd) || defined (L_ge_dd) || defined (L_ge_td)
int
DFP_GE (DFP_C_TYPE arg_a, DFP_C_TYPE arg_b)
{
  int stat;
  stat = dfp_compare_op (decNumberCompare, arg_a, arg_b);
  /* For GE return 1 (>=0) for true, -1 for false.  */
  return (stat != -1) ? 1 : -1;
}
#endif /* L_ge */
