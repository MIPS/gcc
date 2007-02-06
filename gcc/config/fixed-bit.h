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

#ifndef _FIXED_BIT_H
#define _FIXED_BIT_H

/* Permit the tm.h file to select the endianness to use just for this
   file.  This is used when the endianness is determined when the
   compiler is run.  */

#ifndef LIBGCC2_WORDS_BIG_ENDIAN
#define LIBGCC2_WORDS_BIG_ENDIAN WORDS_BIG_ENDIAN
#endif

/* To use this file, we need to define one of the following:
   QQ_MODE, UQQ_MODE, HQ_MODE, UHQ_MODE, SQ_MODE, USQ_MODE, DQ_MODE, UDQ_MODE,
   TQ_MODE, UTQ_MODE, HA_MODE, UHA_MODE, SA_MODE, USA_MODE, DA_MODE, UDA_MODE,
   TA_MODE, UTA_MODE.
   Then, all operators for this machine mode will be created.  */

typedef          int QItype     __attribute__ ((mode (QI)));
typedef unsigned int UQItype    __attribute__ ((mode (QI)));
typedef          int HItype     __attribute__ ((mode (HI)));
typedef unsigned int UHItype    __attribute__ ((mode (HI)));
typedef          _Fract QQtype  __attribute__ ((mode (QQ)));
typedef unsigned _Fract UQQtype __attribute__ ((mode (UQQ)));
typedef          _Fract HQtype  __attribute__ ((mode (HQ)));
typedef unsigned _Fract UHQtype __attribute__ ((mode (UHQ)));
typedef          _Fract HAtype  __attribute__ ((mode (HA)));
typedef unsigned _Fract UHAtype __attribute__ ((mode (UHA)));
#define HAVE_QQ		1
#define HAVE_UQQ	1
#define HAVE_HQ		1
#define HAVE_UHQ	1
#define HAVE_HA		1
#define HAVE_UHA	1
#if MIN_UNITS_PER_WORD > 1
/* These typedefs are usually forbidden on dsp's with UNITS_PER_WORD 1.  */
typedef          int SItype     __attribute__ ((mode (SI)));
typedef unsigned int USItype    __attribute__ ((mode (SI)));
typedef          _Fract SQtype  __attribute__ ((mode (SQ)));
typedef unsigned _Fract USQtype __attribute__ ((mode (USQ)));
typedef          _Fract SAtype  __attribute__ ((mode (SA)));
typedef unsigned _Fract USAtype __attribute__ ((mode (USA)));
#define HAVE_SQ		1
#define HAVE_USQ	1
#define HAVE_SA		1
#define HAVE_USA	1
#define HAVE_SI		1
#if LONG_LONG_TYPE_SIZE > 32
/* These typedefs are usually forbidden on archs with UNITS_PER_WORD 2.  */
typedef          int DItype     __attribute__ ((mode (DI)));
typedef unsigned int UDItype    __attribute__ ((mode (DI)));
typedef          _Fract DQtype  __attribute__ ((mode (DQ)));
typedef unsigned _Fract UDQtype __attribute__ ((mode (UDQ)));
typedef          _Fract DAtype  __attribute__ ((mode (DA)));
typedef unsigned _Fract UDAtype __attribute__ ((mode (UDA)));
#define HAVE_DQ		1
#define HAVE_UDQ	1
#define HAVE_DA		1
#define HAVE_UDA	1
#define HAVE_DI		1
#if MIN_UNITS_PER_WORD > 4
/* These typedefs are usually forbidden on archs with UNITS_PER_WORD 4.  */
typedef          int TItype     __attribute__ ((mode (TI)));
typedef unsigned int UTItype    __attribute__ ((mode (TI)));
typedef          _Fract TQtype  __attribute__ ((mode (TQ)));
typedef unsigned _Fract UTQtype __attribute__ ((mode (UTQ)));
typedef          _Fract TAtype  __attribute__ ((mode (TA)));
typedef unsigned _Fract UTAtype __attribute__ ((mode (UTA)));
#define HAVE_TQ		1
#define HAVE_UTQ	1
#define HAVE_TA		1
#define HAVE_UTA	1
#define HAVE_TI		1
#endif
#endif
#endif

typedef int word_type __attribute__ ((mode (__word__)));

/* Based on modes, we create many defines.  */

#if defined (QQ_MODE) && (HAVE_QQ == 1)
#define FIXED_SIZE	1	/* in bytes.  */
#define INT_C_TYPE	QItype
#define UINT_C_TYPE	UQItype
#define DINT_C_TYPE	HItype
#define DUINT_C_TYPE	UHItype
#define MODE_NAME	QQ
#define MODE_NAME_S	qq
#define MODE_UNSIGNED	0
#endif

#if defined (UQQ_MODE) && (HAVE_UQQ == 1)
#define FIXED_SIZE	1	/* in bytes.  */
#define INT_C_TYPE	UQItype
#define UINT_C_TYPE	UQItype
#define DINT_C_TYPE	UHItype
#define DUINT_C_TYPE	UHItype
#define MODE_NAME	UQQ
#define MODE_NAME_S	uqq
#define MODE_UNSIGNED	1
#endif

#if defined (HQ_MODE) && (HAVE_HQ == 1)
#define FIXED_SIZE	2	/* in bytes.  */
#define INT_C_TYPE	HItype
#define UINT_C_TYPE	UHItype

#if HAVE_SI == 1
#define DINT_C_TYPE	SItype
#define DUINT_C_TYPE	USItype
#else
#define HINT_C_TYPE	QItype
#define HUINT_C_TYPE	UQItype
#endif

#define MODE_NAME	HQ
#define MODE_NAME_S	hq
#define MODE_UNSIGNED	0
#endif

#if defined (UHQ_MODE) && (HAVE_UHQ == 1)
#define FIXED_SIZE	2	/* in bytes.  */
#define INT_C_TYPE	UHItype
#define UINT_C_TYPE	UHItype

#if HAVE_SI == 1
#define DINT_C_TYPE	USItype
#define DUINT_C_TYPE	USItype
#else
#define HINT_C_TYPE	UQItype
#define HUINT_C_TYPE	UQItype
#endif

#define MODE_NAME	UHQ
#define MODE_NAME_S	uhq
#define MODE_UNSIGNED	1
#endif

#if defined (SQ_MODE) && (HAVE_SQ == 1)
#define FIXED_SIZE	4	/* in bytes.  */
#define INT_C_TYPE	SItype
#define UINT_C_TYPE	USItype

#if HAVE_DI == 1
#define DINT_C_TYPE	DItype
#define DUINT_C_TYPE	UDItype
#else
#define HINT_C_TYPE	HItype
#define HUINT_C_TYPE	UHItype
#endif

#define MODE_NAME	SQ
#define MODE_NAME_S	sq
#define MODE_UNSIGNED	0
#endif

#if defined (USQ_MODE) && (HAVE_USQ == 1)
#define FIXED_SIZE	4	/* in bytes.  */
#define INT_C_TYPE	USItype
#define UINT_C_TYPE	USItype

#if HAVE_DI == 1
#define DINT_C_TYPE	UDItype
#define DUINT_C_TYPE	UDItype
#else
#define HINT_C_TYPE	UHItype
#define HUINT_C_TYPE	UHItype
#endif

#define MODE_NAME	USQ
#define MODE_NAME_S	usq
#define MODE_UNSIGNED	1
#endif

#if defined (DQ_MODE) && (HAVE_DQ == 1)
#define FIXED_SIZE	8	/* in bytes.  */
#define INT_C_TYPE	DItype
#define UINT_C_TYPE	UDItype

#if HAVE_TI == 1
#define DINT_C_TYPE	TItype
#define DUINT_C_TYPE	UTItype
#else
#define HINT_C_TYPE	SItype
#define HUINT_C_TYPE	USItype
#endif

#define MODE_NAME	DQ
#define MODE_NAME_S	dq
#define MODE_UNSIGNED	0
#endif

#if defined (UDQ_MODE) && (HAVE_UDQ == 1)
#define FIXED_SIZE	8	/* in bytes.  */
#define INT_C_TYPE	UDItype
#define UINT_C_TYPE	UDItype

#if HAVE_TI == 1
#define DINT_C_TYPE	UTItype
#define DUINT_C_TYPE	UTItype
#else
#define HINT_C_TYPE	USItype
#define HUINT_C_TYPE	USItype
#endif

#define MODE_NAME	UDQ
#define MODE_NAME_S	udq
#define MODE_UNSIGNED	1
#endif

#if defined (TQ_MODE) && (HAVE_TQ == 1)
#define FIXED_SIZE	16	/* in bytes.  */
#define INT_C_TYPE	TItype
#define UINT_C_TYPE	UTItype
#define HINT_C_TYPE	DItype
#define HUINT_C_TYPE	UDItype
#define MODE_NAME	TQ
#define MODE_NAME_S	tq
#define MODE_UNSIGNED	0
#endif

#if defined (UTQ_MODE) && (HAVE_UTQ == 1)
#define FIXED_SIZE	16	/* in bytes.  */
#define INT_C_TYPE	UTItype
#define UINT_C_TYPE	UTItype
#define HINT_C_TYPE	UDItype
#define HUINT_C_TYPE	UDItype
#define MODE_NAME	UTQ
#define MODE_NAME_S	utq
#define MODE_UNSIGNED	1
#endif

#if defined (HA_MODE) && (HAVE_HA == 1)
#define FIXED_SIZE	2	/* in bytes.  */
#define INT_C_TYPE	HItype
#define UINT_C_TYPE	UHItype

#if HAVE_SI == 1
#define DINT_C_TYPE	SItype
#define DUINT_C_TYPE	USItype
#else
#define HINT_C_TYPE	QItype
#define HUINT_C_TYPE	UQItype
#endif

#define MODE_NAME	HA
#define MODE_NAME_S	ha
#define MODE_UNSIGNED	0
#endif

#if defined (UHA_MODE) && (HAVE_UHA == 1)
#define FIXED_SIZE	2	/* in bytes.  */
#define INT_C_TYPE	UHItype
#define UINT_C_TYPE	UHItype

#if HAVE_SI == 1
#define DINT_C_TYPE	USItype
#define DUINT_C_TYPE	USItype
#else
#define HINT_C_TYPE	UQItype
#define HUINT_C_TYPE	UQItype
#endif

#define MODE_NAME	UHA
#define MODE_NAME_S	uha
#define MODE_UNSIGNED	1
#endif

#if defined (SA_MODE) && (HAVE_SA == 1)
#define FIXED_SIZE	4	/* in bytes.  */
#define INT_C_TYPE	SItype
#define UINT_C_TYPE	USItype

#if HAVE_DI == 1
#define DINT_C_TYPE	DItype
#define DUINT_C_TYPE	UDItype
#else
#define HINT_C_TYPE	HItype
#define HUINT_C_TYPE	UHItype
#endif

#define MODE_NAME	SA
#define MODE_NAME_S	sa
#define MODE_UNSIGNED	0
#endif

#if defined (USA_MODE) && (HAVE_USA == 1)
#define FIXED_SIZE	4	/* in bytes.  */
#define INT_C_TYPE	USItype
#define UINT_C_TYPE	USItype

#if HAVE_DI == 1
#define DINT_C_TYPE	UDItype
#define DUINT_C_TYPE	UDItype
#else
#define HINT_C_TYPE	UHItype
#define HUINT_C_TYPE	UHItype
#endif

#define MODE_NAME	USA
#define MODE_NAME_S	usa
#define MODE_UNSIGNED	1
#endif

#if defined (DA_MODE) && (HAVE_DA == 1)
#define FIXED_SIZE	8	/* in bytes.  */
#define INT_C_TYPE	DItype
#define UINT_C_TYPE	UDItype

#if HAVE_TI == 1
#define DINT_C_TYPE	TItype
#define DUINT_C_TYPE	UTItype
#else
#define HINT_C_TYPE	SItype
#define HUINT_C_TYPE	USItype
#endif

#define MODE_NAME	DA
#define MODE_NAME_S	da
#define MODE_UNSIGNED	0
#endif

#if defined (UDA_MODE) && (HAVE_UDA == 1)
#define FIXED_SIZE	8	/* in bytes.  */
#define INT_C_TYPE	UDItype
#define UINT_C_TYPE	UDItype

#if HAVE_TI == 1
#define DINT_C_TYPE	UTItype
#define DUINT_C_TYPE	UTItype
#else
#define HINT_C_TYPE	USItype
#define HUINT_C_TYPE	USItype
#endif

#define MODE_NAME	UDA
#define MODE_NAME_S	uda
#define MODE_UNSIGNED	1
#endif

#if defined (TA_MODE) && (HAVE_TA == 1)
#define FIXED_SIZE	16	/* in bytes.  */
#define INT_C_TYPE	TItype
#define UINT_C_TYPE	UTItype
#define HINT_C_TYPE	DItype
#define HUINT_C_TYPE	UDItype
#define MODE_NAME	TA
#define MODE_NAME_S	ta
#define MODE_UNSIGNED	0
#endif

#if defined (UTA_MODE) && (HAVE_UTA == 1)
#define FIXED_SIZE	16	/* in bytes.  */
#define INT_C_TYPE	UTItype
#define UINT_C_TYPE	UTItype
#define HINT_C_TYPE	UDItype
#define HUINT_C_TYPE	UDItype
#define MODE_NAME	UTA
#define MODE_NAME_S	uta
#define MODE_UNSIGNED	1
#endif

/* The following defines are based on the previous defines.  */

#if defined (HINT_C_TYPE)
#if LIBGCC2_WORDS_BIG_ENDIAN
  struct INTstruct {HINT_C_TYPE high, low;};
#else
  struct INTstruct {HINT_C_TYPE low, high;};
#endif

typedef union
{
  struct INTstruct s;
  INT_C_TYPE ll;
} INTunion;
#endif

#define FIXED_WIDTH	(FIXED_SIZE * BITS_PER_UNIT) /* in bits.  */
#define FIXED_C_TYPE1(NAME)	NAME ## type
#define FIXED_C_TYPE2(NAME)	FIXED_C_TYPE1(NAME)
#define FIXED_C_TYPE	FIXED_C_TYPE2(MODE_NAME)
#define FBITS1(NAME)	__ ## NAME ## _FBIT__
#define FBITS2(NAME)	FBITS1(NAME)
#define FBITS		FBITS2(MODE_NAME)
#define IBITS1(NAME)	__ ## NAME ## _IBIT__
#define IBITS2(NAME)	IBITS1(NAME)
#define IBITS		IBITS2(MODE_NAME)
#define I_F_BITS	(FBITS + IBITS)

#define FIXED_OP(OP,MODE,NUM)	OP ## MODE ## NUM

#define FIXED_SATURATE1_TEMP(NAME)	FIXED_OP(__saturate1,NAME,)
#define FIXED_SATURATE2_TEMP(NAME)	FIXED_OP(__saturate2,NAME,)
#define FIXED_MULHELPER_TEMP(NAME)	FIXED_OP(__mulhelper,NAME,)
#define FIXED_DIVHELPER_TEMP(NAME)	FIXED_OP(__divhelper,NAME,)
#define FIXED_ASHLHELPER_TEMP(NAME)	FIXED_OP(__ashlhelper,NAME,)
#define FIXED_ADD_TEMP(NAME)	FIXED_OP(__add,NAME,3)
#define FIXED_SSADD_TEMP(NAME)	FIXED_OP(__ssadd,NAME,3)
#define FIXED_USADD_TEMP(NAME)	FIXED_OP(__usadd,NAME,3)
#define FIXED_SUB_TEMP(NAME)	FIXED_OP(__sub,NAME,3)
#define FIXED_SSSUB_TEMP(NAME)	FIXED_OP(__sssub,NAME,3)
#define FIXED_USSUB_TEMP(NAME)	FIXED_OP(__ussub,NAME,3)
#define FIXED_MUL_TEMP(NAME)	FIXED_OP(__mul,NAME,3)
#define FIXED_SSMUL_TEMP(NAME)	FIXED_OP(__ssmul,NAME,3)
#define FIXED_USMUL_TEMP(NAME)	FIXED_OP(__usmul,NAME,3)
#define FIXED_DIV_TEMP(NAME)	FIXED_OP(__div,NAME,3)
#define FIXED_UDIV_TEMP(NAME)	FIXED_OP(__udiv,NAME,3)
#define FIXED_SSDIV_TEMP(NAME)	FIXED_OP(__ssdiv,NAME,3)
#define FIXED_USDIV_TEMP(NAME)	FIXED_OP(__usdiv,NAME,3)
#define FIXED_NEG_TEMP(NAME)	FIXED_OP(__neg,NAME,2)
#define FIXED_SSNEG_TEMP(NAME)	FIXED_OP(__ssneg,NAME,2)
#define FIXED_USNEG_TEMP(NAME)	FIXED_OP(__usneg,NAME,2)
#define FIXED_ASHL_TEMP(NAME)	FIXED_OP(__ashl,NAME,3)
#define FIXED_ASHR_TEMP(NAME)	FIXED_OP(__ashr,NAME,3)
#define FIXED_LSHR_TEMP(NAME)	FIXED_OP(__lshr,NAME,3)
#define FIXED_SSASHL_TEMP(NAME)	FIXED_OP(__ssashl,NAME,3)
#define FIXED_USASHL_TEMP(NAME)	FIXED_OP(__usashl,NAME,3)
#define FIXED_CMP_TEMP(NAME)	FIXED_OP(__cmp,NAME,2)

#if defined (MODE_NAME)
#if defined (DINT_C_TYPE)
#define FIXED_SATURATE1	FIXED_SATURATE1_TEMP(MODE_NAME_S)
#else
#define FIXED_SATURATE2	FIXED_SATURATE2_TEMP(MODE_NAME_S)
#endif
#define FIXED_MULHELPER	FIXED_MULHELPER_TEMP(MODE_NAME_S)
#define FIXED_DIVHELPER	FIXED_DIVHELPER_TEMP(MODE_NAME_S)
#define FIXED_ASHLHELPER	FIXED_ASHLHELPER_TEMP(MODE_NAME_S)
#define FIXED_ADD	FIXED_ADD_TEMP(MODE_NAME_S)
#define FIXED_SUB	FIXED_SUB_TEMP(MODE_NAME_S)
#define FIXED_MUL	FIXED_MUL_TEMP(MODE_NAME_S)
#define FIXED_NEG	FIXED_NEG_TEMP(MODE_NAME_S)
#define FIXED_ASHL	FIXED_ASHL_TEMP(MODE_NAME_S)
#define FIXED_CMP	FIXED_CMP_TEMP(MODE_NAME_S)

/* The following functions are for all fixed-point modes.  */
#if defined (DINT_C_TYPE)
extern void FIXED_SATURATE1 (DINT_C_TYPE *);
#else
extern void FIXED_SATURATE2 (INT_C_TYPE *, INT_C_TYPE *);
#endif
extern FIXED_C_TYPE FIXED_MULHELPER (FIXED_C_TYPE, FIXED_C_TYPE, word_type);
extern FIXED_C_TYPE FIXED_DIVHELPER (FIXED_C_TYPE, FIXED_C_TYPE, word_type);
extern FIXED_C_TYPE FIXED_ASHLHELPER (FIXED_C_TYPE, word_type, word_type);
extern FIXED_C_TYPE FIXED_ADD (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_SUB (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_MUL (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_NEG (FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_ASHL (FIXED_C_TYPE, word_type);
extern word_type FIXED_CMP (FIXED_C_TYPE, FIXED_C_TYPE);
#endif

#if MODE_UNSIGNED == 0 /* Signed types.  */
#define PADDING_BITS	(FIXED_WIDTH - 1 - I_F_BITS)
#define NONPADDING_BITS	(1 + I_F_BITS)

#if defined (MODE_NAME)
#define FIXED_DIV	FIXED_DIV_TEMP(MODE_NAME_S)
#define FIXED_ASHR	FIXED_ASHR_TEMP(MODE_NAME_S)
#define FIXED_SSADD	FIXED_SSADD_TEMP(MODE_NAME_S)
#define FIXED_SSSUB	FIXED_SSSUB_TEMP(MODE_NAME_S)
#define FIXED_SSMUL	FIXED_SSMUL_TEMP(MODE_NAME_S)
#define FIXED_SSDIV	FIXED_SSDIV_TEMP(MODE_NAME_S)
#define FIXED_SSNEG	FIXED_SSNEG_TEMP(MODE_NAME_S)
#define FIXED_SSASHL	FIXED_SSASHL_TEMP(MODE_NAME_S)

/* The following functions are for signed fixed-point modes.  */
extern FIXED_C_TYPE FIXED_DIV (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_ASHR (FIXED_C_TYPE, word_type);
extern FIXED_C_TYPE FIXED_SSADD (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_SSSUB (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_SSMUL (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_SSDIV (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_SSNEG (FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_SSASHL (FIXED_C_TYPE, word_type);
#endif

#else /* Unsigned types.  */
#define PADDING_BITS	(FIXED_WIDTH - I_F_BITS)
#define NONPADDING_BITS	(I_F_BITS)

#if defined (MODE_NAME)
#define FIXED_UDIV	FIXED_UDIV_TEMP(MODE_NAME_S)
#define FIXED_LSHR	FIXED_LSHR_TEMP(MODE_NAME_S)
#define FIXED_USDIV	FIXED_USDIV_TEMP(MODE_NAME_S)
#define FIXED_USADD	FIXED_USADD_TEMP(MODE_NAME_S)
#define FIXED_USSUB	FIXED_USSUB_TEMP(MODE_NAME_S)
#define FIXED_USMUL	FIXED_USMUL_TEMP(MODE_NAME_S)
#define FIXED_USNEG	FIXED_USNEG_TEMP(MODE_NAME_S)
#define FIXED_USASHL	FIXED_USASHL_TEMP(MODE_NAME_S)

/* The following functions are for unsigned fixed-point modes.  */
extern FIXED_C_TYPE FIXED_UDIV (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_LSHR (FIXED_C_TYPE, word_type);
extern FIXED_C_TYPE FIXED_USADD (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_USSUB (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_USMUL (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_USDIV (FIXED_C_TYPE, FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_USNEG (FIXED_C_TYPE);
extern FIXED_C_TYPE FIXED_USASHL (FIXED_C_TYPE, word_type);
#endif

#endif /* End of testing MODE_UNSIGNED.  */

/* This define is to check if this mode have any padding bits.  */
#define HAVE_PADDING_BITS	(PADDING_BITS > 0)

#endif  /* _FIXED_BIT_H */
