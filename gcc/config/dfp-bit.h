/* Header file for dfp-bit.c.
   Copyright (C) 2005 Free Software Foundation, Inc.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef _DFPBIT_H
#define _DFPBIT_H

#include "decNumber.h"

/* Depending on WIDTH, define a number of macros:

   DFP_TYPE: name of the arguments to the libgcc functions;
   
   TO_INTEGRAL: the name of the decNumber function to convert an
   encoded value into the decNumber internal representation;

   TO_ENCODED: the name of the decNumber function to convert an
   internally represented decNumber into the encoded
   representation.  */

#if WIDTH == 32
#include "decimal32.h"
#define DFP_TYPE	decimal32
#define TO_INTERNAL	decimal32ToNumber
#define TO_ENCODED	decimal32FromNumber
#elif WIDTH == 64
#include "decimal64.h"
#define DFP_TYPE	decimal64
#define TO_INTERNAL	decimal64ToNumber
#define TO_ENCODED	decimal64FromNumber
#elif WIDTH == 128
#include "decimal128.h"
#define DFP_TYPE	decimal128
#define TO_INTERNAL	decimal128ToNumber
#define TO_ENCODED	decimal128FromNumber
#else
#error invalid decimal float width
#endif

#if WIDTH == 32
#define DFP_ADD		dfp_add32
#define DFP_SUB		dfp_sub32
#define DFP_MULTIPLY	dfp_multiply32
#define DFP_DIVIDE	dfp_divide32
#define DFP_PLUS	dfp_plus32
#define DFP_MINUS	dfp_minus32
#define DFP_EQ		dfp_eq32
#define DFP_NE		dfp_ne32
#define DFP_LT		dfp_lt32
#define DFP_GT		dfp_gt32
#define DFP_LE		dfp_le32
#define DFP_GE		dfp_ge32
#elif WIDTH == 64
#define DFP_ADD		dfp_add64
#define DFP_SUB		dfp_sub64
#define DFP_MULTIPLY	dfp_multiply64
#define DFP_DIVIDE	dfp_divide64
#define DFP_PLUS	dfp_plus64
#define DFP_MINUS	dfp_minus64
#define DFP_EQ		dfp_eq64
#define DFP_NE		dfp_ne64
#define DFP_LT		dfp_lt64
#define DFP_GT		dfp_gt64
#define DFP_LE		dfp_le64
#define DFP_GE		dfp_ge64
#elif WIDTH == 128
#define DFP_ADD		dfp_add128
#define DFP_SUB		dfp_sub128
#define DFP_MULTIPLY	dfp_multiply128
#define DFP_DIVIDE	dfp_divide128
#define DFP_PLUS	dfp_plus128
#define DFP_MINUS	dfp_minus128
#define DFP_EQ		dfp_eq128
#define DFP_NE		dfp_ne128
#define DFP_LT		dfp_lt128
#define DFP_GT		dfp_gt128
#define DFP_LE		dfp_le128
#define DFP_GE		dfp_ge128
#endif

/* Prototypes.  */

#if defined (L_mul_sd) || defined (L_mul_dd) || defined (L_mul_td)
extern DFP_TYPE DFP_MULTIPLY (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_addsub_sd) || defined (L_addsub_dd) || defined (L_addsub_td)
extern DFP_TYPE DFP_ADD (DFP_TYPE, DFP_TYPE);
extern DFP_TYPE DFP_SUB (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_plus_sd) || defined (L_plus_dd) || defined (L_plus_td)
extern DFP_TYPE DFP_PLUS (DFP_TYPE);
#endif

#if defined (L_minus_sd) || defined (L_minus_dd) || defined (L_minus_td)
extern DFP_TYPE DFP_MINUS (DFP_TYPE);
#endif

#if defined (L_eq_sd) || defined (L_eq_dd) || defined (L_eq_td)
extern int DFP_EQ (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_ne_sd) || defined (L_ne_dd) || defined (L_ne_td)
extern int DFP_NE (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_lt_sd) || defined (L_lt_dd) || defined (L_lt_td)
extern int DFP_LT (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_gt_sd) || defined (L_gt_dd) || defined (L_gt_td)
extern int DFP_GT (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_le_sd) || defined (L_le_dd) || defined (L_le_td)
extern int DFP_LE (DFP_TYPE, DFP_TYPE);
#endif

#if defined (L_ge_sd) || defined (L_ge_dd) || defined (L_ge_td)
extern int DFP_GE (DFP_TYPE, DFP_TYPE);
#endif

#endif /* _DFPBIT_H */
