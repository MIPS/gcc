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

/* Depending on WIDTH, define a number of macros:

   DFP_C_TYPE: type of the arguments to the libgcc functions;
	(eg _Decimal32)

   DFP_TYPE: the corresponding (encoded) IEEE754R type;
	(eg decimal32)
   
   TO_INTERNAL: the name of the decNumber function to convert an
   encoded value into the decNumber internal representation;

   TO_ENCODED: the name of the decNumber function to convert an
   internally represented decNumber into the encoded
   representation.  */

#if WIDTH == 32
#include "decimal32.h"
#define DFP_C_TYPE	_Decimal32
#define DFP_TYPE	decimal32
#define TO_INTERNAL	decimal32ToNumber
#define TO_ENCODED	decimal32FromNumber
#elif WIDTH == 64
#include "decimal64.h"
#define DFP_C_TYPE	_Decimal64
#define DFP_TYPE	decimal64
#define TO_INTERNAL	decimal64ToNumber
#define TO_ENCODED	decimal64FromNumber
#elif WIDTH == 128
#include "decimal128.h"
#define DFP_C_TYPE	_Decimal128
#define DFP_TYPE	decimal128
#define TO_INTERNAL	decimal128ToNumber
#define TO_ENCODED	decimal128FromNumber
#else
#error invalid decimal float word width
#endif

/* We define __DEC_EVAL_METHOD__ to 2, saying that we evaluate all
   operations and constants to the range and precision of the _Decimal32
   type.  Make it so.  */
#define CONTEXT_INIT DEC_INIT_DECIMAL128
                                                                                
/* Define a rounding mode to use everywhere, using the default for
   decNumber which is also the default in the draft IEEE 754r.  */
#if 1
#define CONTEXT_ROUND DEC_ROUND_HALF_EVEN
#define CONTEXT_TRAPS 0
#else
/* For testing we can replace the constant with a variable reference,
   which currently must be defined in a separate shared library linked
   with tests.  */
extern int __dfp_round;
#define CONTEXT_ROUND __dfp_round
extern int __dfp_traps;
#define CONTEXT_TRAPS __dfp_traps
#endif

#include "decNumber.h"

#if WIDTH == 32
#define DFP_ADD		__addsd3
#define DFP_SUB		__subsd3
#define DFP_MULTIPLY	__mulsd3
#define DFP_DIVIDE	__divsd3
#define DFP_EQ		__eqsd2
#define DFP_NE		__nesd2
#define DFP_LT		__ltsd2
#define DFP_GT		__gtsd2
#define DFP_LE		__lesd2
#define DFP_GE		__gesd2
#elif WIDTH == 64
#define DFP_ADD		__adddd3
#define DFP_SUB		__subdd3
#define DFP_MULTIPLY	__muldd3
#define DFP_DIVIDE	__divdd3
#define DFP_EQ		__eqdd2
#define DFP_NE		__nedd2
#define DFP_LT		__ltdd2
#define DFP_GT		__gtdd2
#define DFP_LE		__ledd2
#define DFP_GE		__gedd2
#elif WIDTH == 128
#define DFP_ADD		__addtd3
#define DFP_SUB		__subtd3
#define DFP_MULTIPLY	__multd3
#define DFP_DIVIDE	__divtd3
#define DFP_EQ		__eqtd2
#define DFP_NE		__netd2
#define DFP_LT		__lttd2
#define DFP_GT		__gttd2
#define DFP_LE		__letd2
#define DFP_GE		__getd2
#endif

/* Prototypes.  */

#if defined (L_mul_sd) || defined (L_mul_dd) || defined (L_mul_td)
extern DFP_C_TYPE DFP_MULTIPLY (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_div_sd) || defined (L_div_dd) || defined (L_div_td)
extern DFP_C_TYPE DFP_DIVIDE (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_addsub_sd) || defined (L_addsub_dd) || defined (L_addsub_td)
extern DFP_C_TYPE DFP_ADD (DFP_C_TYPE, DFP_C_TYPE);
extern DFP_C_TYPE DFP_SUB (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_eq_sd) || defined (L_eq_dd) || defined (L_eq_td)
extern int DFP_EQ (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_ne_sd) || defined (L_ne_dd) || defined (L_ne_td)
extern int DFP_NE (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_lt_sd) || defined (L_lt_dd) || defined (L_lt_td)
extern int DFP_LT (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_gt_sd) || defined (L_gt_dd) || defined (L_gt_td)
extern int DFP_GT (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_le_sd) || defined (L_le_dd) || defined (L_le_td)
extern int DFP_LE (DFP_C_TYPE, DFP_C_TYPE);
#endif

#if defined (L_ge_sd) || defined (L_ge_dd) || defined (L_ge_td)
extern int DFP_GE (DFP_C_TYPE, DFP_C_TYPE);
#endif

#endif /* _DFPBIT_H */
