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
   representation.

   FROM_STRING: the name of the decNumber function to read an
   encoded value from a string.

   TO_STRING: the name of the decNumber function to write an
   encoded value to a string.  */

#if WIDTH == 32
#define DFP_C_TYPE	_Decimal32
#define DFP_TYPE	decimal32
#define TO_INTERNAL	decimal32ToNumber
#define TO_ENCODED	decimal32FromNumber
#define FROM_STRING	decimal32FromString
#define TO_STRING	decimal32ToString
#elif WIDTH == 64
#define DFP_C_TYPE	_Decimal64
#define DFP_TYPE	decimal64
#define TO_INTERNAL	decimal64ToNumber
#define TO_ENCODED	decimal64FromNumber
#define FROM_STRING	decimal64FromString
#define TO_STRING	decimal64ToString
#elif WIDTH == 128
#define DFP_C_TYPE	_Decimal128
#define DFP_TYPE	decimal128
#define TO_INTERNAL	decimal128ToNumber
#define TO_ENCODED	decimal128FromNumber
#define FROM_STRING	decimal128FromString
#define TO_STRING	decimal128ToString
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
#define CONTEXT_ROUND	DEC_ROUND_HALF_EVEN
#define CONTEXT_TRAPS	0
#define DFP_RAISE(A)
#else
/* For testing we can replace the constant with a variable reference,
   which currently must be defined in a separate shared library linked
   with tests.  */
extern int __dfp_round;
#define CONTEXT_ROUND	__dfp_round
extern int __dfp_traps;
#define CONTEXT_TRAPS	__dfp_traps
extern void __dfp_raise (int);
#define DFP_RAISE(A)	__dfp_raise(A)
#endif

/* Conversions between different decimal float types use WIDTH_TO to
   determine additional macros to define.  */

#if defined (L_dd_to_sd) || defined (L_td_to_sd)
#define WIDTH_TO 32
#elif defined (L_sd_to_dd) || defined (L_td_to_dd)
#define WIDTH_TO 64
#elif defined (L_sd_to_td) || defined (L_dd_to_td)
#define WIDTH_TO 128
#endif

/* If WIDTH_TO is defined, define additional macros:

   DFP_C_TYPE_TO: type of the result of dfp to dfp conversion.

   DFP_TYPE_TO: the corresponding (encoded) IEEE754R type.

   TO_ENCODED: the name of the decNumber function to convert an
   internally represented decNumber into the encoded representation
   for the destination.  */

#if WIDTH_TO == 32
#define DFP_C_TYPE_TO	_Decimal32
#define DFP_TYPE_TO	decimal32
#undef TO_ENCODED
#define TO_ENCODED	decimal32FromNumber
#elif WIDTH_TO == 64
#define DFP_C_TYPE_TO	_Decimal64
#define DFP_TYPE_TO	decimal64
#undef TO_ENCODED
#define TO_ENCODED	decimal64FromNumber
#elif WIDTH_TO == 128
#define DFP_C_TYPE_TO	_Decimal128
#define DFP_TYPE_TO	decimal128
#undef TO_ENCODED
#define TO_ENCODED	decimal128FromNumber
#endif

/* Conversions between decimal float types and integral types use INT_KIND
   to determine the data type and C functions to use.  */

#if defined (L_sd_to_si) || defined (L_dd_to_si) || defined (L_td_to_si)  \
   || defined (L_si_to_sd) || defined (L_si_to_dd) || defined (L_si_to_td)
#define INT_KIND 1
#elif defined (L_sd_to_di) || defined (L_dd_to_di) || defined (L_td_to_di) \
   || defined (L_di_to_sd) || defined (L_di_to_dd) || defined (L_di_to_td)
#define INT_KIND 2
#elif defined (L_sd_to_usi) || defined (L_dd_to_usi) || defined (L_td_to_usi) \
   || defined (L_usi_to_sd) || defined (L_usi_to_dd) || defined (L_usi_to_td)
#define INT_KIND 3
#elif defined (L_sd_to_udi) || defined (L_dd_to_udi) || defined (L_td_to_udi) \
   || defined (L_udi_to_sd) || defined (L_udi_to_dd) || defined (L_udi_to_td)
#define INT_KIND 4
#endif

/*  If INT_KIND is defined, define additional macros:

    INT_TYPE: The integer data type.

    INT_FMT: The format string for writing the integer to a string.

    STR_TO_INT: The function to read the integer from a string.  */

#if INT_KIND == 1
typedef int SItype __attribute__ ((mode (SI)));
#define INT_TYPE SItype
#define INT_FMT "%d"
#define STR_TO_INT strtol
#elif INT_KIND == 2
typedef int DItype __attribute__ ((mode (DI)));
#define INT_TYPE DItype
#define INT_FMT "%lld"
#define STR_TO_INT strtoll
#elif INT_KIND == 3
typedef unsigned int USItype __attribute__ ((mode (SI)));
#define INT_TYPE USItype
#define INT_FMT "%u"
#define STR_TO_INT strtoul
#elif INT_KIND == 4
typedef unsigned int UDItype __attribute__ ((mode (DI)));
#define INT_TYPE UDItype
#define INT_FMT "%llu"
#define STR_TO_INT strtoull
#endif

/* Conversions between decimal float types and binary float types use
   BFP_KIND to determine the data type and C functions to use.  */

#if defined (L_sd_to_sf) || defined (L_dd_to_sf) || defined (L_td_to_sf) \
 || defined (L_sf_to_sd) || defined (L_sf_to_dd) || defined (L_sf_to_td)
#define BFP_KIND 1
#elif defined (L_sd_to_df) || defined (L_dd_to_df ) || defined (L_td_to_df) \
 ||   defined (L_df_to_sd) || defined (L_df_to_dd) || defined (L_df_to_td)
#define BFP_KIND 2
#endif

/*  If BFP_KIND is defined, define additional macros:

    BFP_TYPE: The binary floating point data type.

    BFP_FMT: The format string for writing the value to a string.

    STR_TO_BFP: The function to read the value from a string.  */

#if BFP_KIND == 1
typedef float SFtype __attribute__ ((mode (SF)));
/* strtof is declared in <stdlib.h> only for C99.  */
extern float strtof (const char *, char **);
#define BFP_TYPE SFtype
#define BFP_FMT "%e"
#define STR_TO_BFP strtof
#elif BFP_KIND == 2
typedef float DFtype __attribute__ ((mode (DF)));
#define BFP_TYPE DFtype
#define BFP_FMT "%e"
#define STR_TO_BFP strtod
#endif

#if WIDTH == 128 || WIDTH_TO == 128
#include "decimal128.h"
#elif WIDTH == 64 || WIDTH_TO == 64
#include "decimal64.h"
#elif WIDTH == 32 || WIDTH_TO == 32
#include "decimal32.h"
#endif

#include "decNumber.h"

/* Names of arithmetic functions.  */

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

/* Names of functions to convert between different decimal float types.  */

#if WIDTH == 32
#if WIDTH_TO == 64
#define DFP_TO_DFP	__extendsddd2
#elif WIDTH_TO == 128
#define DFP_TO_DFP	__extendsdtd2
#endif
#elif WIDTH == 64	
#if WIDTH_TO == 32
#define DFP_TO_DFP	__truncddsd2
#elif WIDTH_TO == 128
#define DFP_TO_DFP	__extendddtd2
#endif
#elif WIDTH == 128
#define DFP_TO_DFP	__trunctdsd2
#if WIDTH_TO == 32
#define DFP_TO_DFP	__trunctddd2
#elif WIDTH == 64
#endif
#endif

/* Names of functions to convert between decimal float and integers.  */

#if WIDTH == 32
#if INT_KIND == 1
#define INT_TO_DFP	__floatsisd
#define DFP_TO_INT	__fixsdsi
#elif INT_KIND == 2
#define INT_TO_DFP	__floatdisd
#define DFP_TO_INT	__fixsddi
#elif INT_KIND == 3
#define INT_TO_DFP	__floatunssisd
#define DFP_TO_INT	__fixunssdsi
#elif INT_KIND == 4
#define INT_TO_DFP	__floatunsdisd
#define DFP_TO_INT	__fixunssddi
#endif
#elif WIDTH == 64
#if INT_KIND == 1
#define INT_TO_DFP	__floatsidd
#define DFP_TO_INT	__fixddsi
#elif INT_KIND == 2
#define INT_TO_DFP	__floatdidd
#define DFP_TO_INT	__fixdddi
#elif INT_KIND == 3
#define INT_TO_DFP	__floatunssidd
#define DFP_TO_INT	__fixunsddsi
#elif INT_KIND == 4
#define INT_TO_DFP	__floatunsdidd
#define DFP_TO_INT	__fixunsdddi
#endif
#elif WIDTH == 128
#if INT_KIND == 1
#define INT_TO_DFP	__floatsitd
#define DFP_TO_INT	__fixtdsi
#elif INT_KIND == 2
#define INT_TO_DFP	__floatditd
#define DFP_TO_INT	__fixtddi
#elif INT_KIND == 3
#define INT_TO_DFP	__floatunssitd
#define DFP_TO_INT	__fixunstdsi
#elif INT_KIND == 4
#define INT_TO_DFP	__floatunsditd
#define DFP_TO_INT	__fixunstddi
#endif
#endif

/* Names of functions to convert between decimal float and binary float.  */

#if WIDTH == 32
#if BFP_KIND == 1
#define BFP_TO_DFP	__extendsfsd2
#define DFP_TO_BFP	__truncsdsf2
#elif BFP_KIND == 2
#define BFP_TO_DFP	__truncdfsd2
#define DFP_TO_BFP	__extendsddf2
#endif
#elif WIDTH == 64
#if BFP_KIND == 1
#define BFP_TO_DFP	__extendsfdd2
#define DFP_TO_BFP	__truncddsf2
#elif BFP_KIND == 2
#define BFP_TO_DFP	__extenddfdd2
#define DFP_TO_BFP	__truncdddf2
#endif
#elif WIDTH == 128
#if BFP_KIND == 1
#define BFP_TO_DFP	__extendsftd2
#define DFP_TO_BFP	__trunctdsf2
#elif BFP_KIND == 2
#define BFP_TO_DFP	__extenddftd2
#define DFP_TO_BFP	__trunctddf2
#endif
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

#if defined (L_sd_to_dd) || defined (L_sd_to_td) || defined (L_dd_to_sd) \
 || defined (L_dd_to_td) || defined (L_td_to_sd) || defined (L_td_to_dd)
extern DFP_C_TYPE_TO DFP_TO_DFP (DFP_C_TYPE);
#endif

#if defined (L_sd_to_si) || defined (L_dd_to_si) || defined (L_td_to_si) \
 || defined (L_sd_to_di) || defined (L_dd_to_di) || defined (L_td_to_di) \
 || defined (L_sd_to_usi) || defined (L_dd_to_usi) || defined (L_td_to_usi) \
 || defined (L_sd_to_udi) || defined (L_dd_to_udi) || defined (L_td_to_udi)
extern INT_TYPE DFP_TO_INT (DFP_C_TYPE);
#endif

#if defined (L_si_to_sd) || defined (L_si_to_dd) || defined (L_si_to_td) \
 || defined (L_di_to_sd) || defined (L_di_to_dd) || defined (L_di_to_td) \
 || defined (L_usi_to_sd) || defined (L_usi_to_dd) || defined (L_usi_to_td) \
 || defined (L_udi_to_sd) || defined (L_udi_to_dd) || defined (L_udi_to_td)
extern DFP_C_TYPE INT_TO_DFP (INT_TYPE);
#endif

#if defined (L_sd_to_sf) || defined (L_dd_to_sf) || defined (L_td_to_sf) \
 || defined (L_sd_to_df) || defined (L_dd_to_df) || defined (L_td_to_df)
extern BFP_TYPE DFP_TO_BFP (DFP_C_TYPE);
#endif

#if defined (L_sf_to_sd) || defined (L_sf_to_dd) || defined (L_sf_to_td) \
 || defined (L_df_to_sd) || defined (L_df_to_dd) || defined (L_df_to_td)
extern DFP_C_TYPE BFP_TO_DFP (BFP_TYPE);
#endif

#endif /* _DFPBIT_H */
