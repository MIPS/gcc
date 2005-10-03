/* Decimal 64-bit format module for the decNumber C Library
   Copyright (C) 2005 Free Software Foundation, Inc.
   Contributed by IBM Corporation.  Author Mike Cowlishaw.

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

/* ------------------------------------------------------------------ */
/* This module comprises the routines for decimal64 format numbers.   */
/* Conversions are supplied to and from decNumber and String.         */
/*                                                                    */
/* No arithmetic routines are included; decNumber provides these.     */
/*                                                                    */
/* Error handling is the same as decNumber (qv.).                     */
/* ------------------------------------------------------------------ */
#include <string.h>		/* [for memset/memcpy] */
#include <stdio.h>		/* [for printf] */

#define  DECNUMDIGITS 16	/* we need decNumbers with space for 16 */
#include "decNumber.h"		/* base number library */
#include "decNumberLocal.h"	/* decNumber local types, etc. */
#include "decimal64.h"		/* our primary include */


/* Utility routines [in decimal64.c] */
extern void decDensePackCoeff (decNumber *, uByte *, Int, Int);
extern void decDenseUnpackCoeff (uByte *, Int, decNumber *, Int, Int);

#if DECTRACE || DECCHECK
void decimal64Show (decimal64 *);	/* for debug */
void decNumberShow (decNumber *);	/* .. */
#endif

/* Useful macro */
/* Clear a structure (e.g., a decNumber) */
#define DEC_clear(d) memset(d, 0, sizeof(*d))

/* ------------------------------------------------------------------ */
/* decimal64FromNumber -- convert decNumber to decimal64              */
/*                                                                    */
/*   ds is the target decimal64                                       */
/*   dn is the source number (assumed valid)                          */
/*   set is the context, used only for reporting errors               */
/*                                                                    */
/* The set argument is used only for status reporting and for the     */
/* rounding mode (used if the coefficient is more than DECIMAL64_Pmax */
/* digits or an overflow is detected).  If the exponent is out of the */
/* valid range then Overflow or Underflow will be raised.             */
/* After Underflow a subnormal result is possible.                    */
/*                                                                    */
/* DEC_Clamped is set if the number has to be 'folded down' to fit,   */
/* by reducing its exponent and multiplying the coefficient by a      */
/* power of ten, or if the exponent on a zero had to be clamped.      */
/* ------------------------------------------------------------------ */
decimal64 *
decimal64FromNumber (decimal64 * d64, decNumber * dn, decContext * set)
{
  uInt status = 0;		/* status accumulator */
  Int pad = 0;			/* coefficient pad digits */
  decNumber dw;			/* work */
  decContext dc;		/* .. */
  uByte isneg = dn->bits & DECNEG;	/* non-0 if original sign set */
  uInt comb, exp;		/* work */

  /* If the number is finite, and has too many digits, or the exponent */
  /* could be out of range then we reduce the number under the */
  /* appropriate constraints */
  if (!(dn->bits & DECSPECIAL))
    {				/* not a special value */
      Int ae = dn->exponent + dn->digits - 1;	/* adjusted exponent */
      if (dn->digits > DECIMAL64_Pmax	/* too many digits */
	  || ae > DECIMAL64_Emax	/* likely overflow */
	  || ae < DECIMAL64_Emin)
	{			/* likely underflow */
	  decContextDefault (&dc, DEC_INIT_DECIMAL64);	/* [no traps] */
	  dc.round = set->round;	/* use supplied rounding */
	  decNumberPlus (&dw, dn, &dc);	/* (round and check) */
	  /* [this changes -0 to 0, but it will be restored below] */
	  status |= dc.status;	/* save status */
	  dn = &dw;		/* use the work number */
	}
      /* [this could have pushed number to Infinity or zero, so this */
      /* rounding must be done before we generate the decimal64] */
    }

  DEC_clear (d64);		/* clean the target */
  if (dn->bits & DECSPECIAL)
    {				/* a special value */
      uByte top;		/* work */
      if (dn->bits & DECINF)
	top = DECIMAL_Inf;
      else
	{			/* sNaN or qNaN */
	  if ((*dn->lsu != 0 || dn->digits > 1)	/* non-zero coefficient */
	      && (dn->digits < DECIMAL64_Pmax))
	    {			/* coefficient fits */
	      decDensePackCoeff (dn, d64->bytes, sizeof (d64->bytes), 0);
	    }
	  if (dn->bits & DECNAN)
	    top = DECIMAL_NaN;
	  else
	    top = DECIMAL_sNaN;
	}
      d64->bytes[0] = top;
    }
  else if (decNumberIsZero (dn))
    {				/* a zero */
      /* set and clamp exponent */
      if (dn->exponent < -DECIMAL64_Bias)
	{
	  exp = 0;
	  status |= DEC_Clamped;
	}
      else
	{
	  exp = dn->exponent + DECIMAL64_Bias;	/* bias exponent */
	  if (exp > DECIMAL64_Ehigh)
	    {			/* top clamp */
	      exp = DECIMAL64_Ehigh;
	      status |= DEC_Clamped;
	    }
	}
      comb = (exp >> 5) & 0x18;	/* combination field */
      d64->bytes[0] = (uByte) (comb << 2);
      exp &= 0xff;		/* remaining exponent bits */
      decimal64SetExpCon (d64, exp);
    }
  else
    {				/* non-zero finite number */
      uInt msd;			/* work */

      /* we have a dn that fits, but it may need to be padded */
      exp = (uInt) (dn->exponent + DECIMAL64_Bias);	/* bias exponent */
      if (exp > DECIMAL64_Ehigh)
	{			/* fold-down case */
	  pad = exp - DECIMAL64_Ehigh;
	  exp = DECIMAL64_Ehigh;	/* [to maximum] */
	  status |= DEC_Clamped;
	}

      decDensePackCoeff (dn, d64->bytes, sizeof (d64->bytes), pad);

      /* save and clear the top digit */
      msd = ((unsigned) d64->bytes[1] >> 2) & 0x0f;
      d64->bytes[1] &= 0x03;
      /* create the combination field */
      if (msd >= 8)
	comb = 0x18 | (msd & 0x01) | ((exp >> 7) & 0x06);
      else
	comb = (msd & 0x07) | ((exp >> 5) & 0x18);
      d64->bytes[0] = (uByte) (comb << 2);
      exp &= 0xff;		/* remaining exponent bits */
      decimal64SetExpCon (d64, exp);
    }

  if (isneg)
    decimal64SetSign (d64, 1);
  if (status != 0)
    decContextSetStatus (set, status);	/* pass on status */

  /*decimal64Show(d64); */
  return d64;
}				/* decimal64FromNumber */

/* ------------------------------------------------------------------ */
/* decimal64ToNumber -- convert decimal64 to decNumber                */
/*   d64 is the source decimal64                                      */
/*   dn is the target number, with appropriate space                  */
/* No error is possible.                                              */
/* ------------------------------------------------------------------ */
decNumber *
decimal64ToNumber (decimal64 * d64, decNumber * dn)
{
  uInt msd;			/* coefficient MSD */
  decimal64 wk;			/* working copy, if needed */
  uInt top = d64->bytes[0] & 0x7f;	/* top byte, less sign bit */
  decNumberZero (dn);		/* clean target */
  /* set the sign if negative */
  if (decimal64Sign (d64))
    dn->bits = DECNEG;

  if (top >= 0x78)
    {				/* is a special */
      if ((top & 0x7c) == (DECIMAL_Inf & 0x7c))
	dn->bits |= DECINF;
      else if ((top & 0x7e) == (DECIMAL_NaN & 0x7e))
	dn->bits |= DECNAN;
      else
	dn->bits |= DECSNAN;
      msd = 0;			/* no top digit */
    }
  else
    {				/* have a finite number */
      uInt comb = top >> 2;	/* combination field */
      uInt exp;			/* exponent */

      if (comb >= 0x18)
	{
	  msd = 8 + (comb & 0x01);
	  exp = (comb & 0x06) << 7;	/* MSBs */
	}
      else
	{
	  msd = comb & 0x07;
	  exp = (comb & 0x18) << 5;
	}
      dn->exponent = exp + decimal64ExpCon (d64) - DECIMAL64_Bias;	/* remove bias */
    }

  /* get the coefficient, unless infinite */
  if (!(dn->bits & DECINF))
    {
      Int bunches = DECIMAL64_Pmax / 3;	/* coefficient full bunches to convert */
      Int odd = 0;		/* assume MSD is 0 (no odd bunch) */
      if (msd != 0)
	{			/* coefficient has leading non-0 digit */
	  /* make a copy of the decimal64, with an extra bunch which has */
	  /* the top digit ready for conversion */
	  wk = *d64;		/* take a copy */
	  wk.bytes[0] = 0;	/* clear all but coecon */
	  wk.bytes[1] &= 0x03;	/* .. */
	  wk.bytes[1] |= (msd << 2);	/* and prefix MSD */
	  odd++;		/* indicate the extra */
	  d64 = &wk;		/* use the work copy */
	}
      decDenseUnpackCoeff (d64->bytes, sizeof (d64->bytes), dn, bunches, odd);
    }
  return dn;
}				/* decimal64ToNumber */

/* ------------------------------------------------------------------ */
/* to-scientific-string -- conversion to numeric string               */
/* to-engineering-string -- conversion to numeric string              */
/*                                                                    */
/*   decimal64ToString(d64, string);                                  */
/*   decimal64ToEngString(d64, string);                               */
/*                                                                    */
/*  d64 is the decimal64 format number to convert                     */
/*  string is the string where the result will be laid out            */
/*                                                                    */
/*  string must be at least 24 characters                             */
/*                                                                    */
/*  No error is possible, and no status can be set.                   */
/* ------------------------------------------------------------------ */
char *
decimal64ToString (decimal64 * d64, char *string)
{
  decNumber dn;			/* work */
  decimal64ToNumber (d64, &dn);
  decNumberToString (&dn, string);
  return string;
}				/* DecSingleToString */

char *
decimal64ToEngString (decimal64 * d64, char *string)
{
  decNumber dn;			/* work */
  decimal64ToNumber (d64, &dn);
  decNumberToEngString (&dn, string);
  return string;
}				/* DecSingleToEngString */

/* ------------------------------------------------------------------ */
/* to-number -- conversion from numeric string                        */
/*                                                                    */
/*   decimal64FromString(result, string, set);                        */
/*                                                                    */
/*  result  is the decimal64 format number which gets the result of   */
/*          the conversion                                            */
/*  *string is the character string which should contain a valid      */
/*          number (which may be a special value)                     */
/*  set     is the context                                            */
/*                                                                    */
/* The context is supplied to this routine is used for error handling */
/* (setting of status and traps) and for the rounding mode, only.     */
/* If an error occurs, the result will be a valid decimal64 NaN.      */
/* ------------------------------------------------------------------ */
decimal64 *
decimal64FromString (decimal64 * result, char *string, decContext * set)
{
  decContext dc;		/* work */
  decNumber dn;			/* .. */

  decContextDefault (&dc, DEC_INIT_DECIMAL64);	/* no traps, please */
  dc.round = set->round;	/* use supplied rounding */

  decNumberFromString (&dn, string, &dc);	/* will round if needed */

  decimal64FromNumber (result, &dn, &dc);
  if (dc.status != 0)
    {				/* something happened */
      decContextSetStatus (set, dc.status);	/* .. pass it on */
    }
  return result;
}				/* decimal64FromString */

#if DECTRACE || DECCHECK
/* ------------------------------------------------------------------ */
/* decimal64Show -- display a single in hexadecimal [debug aid]       */
/*   d64 -- the number to show                                        */
/* ------------------------------------------------------------------ */
/* Also shows sign/cob/expconfields extracted */
void
decimal64Show (decimal64 * d64)
{
  char buf[DECIMAL64_Bytes * 2 + 1];
  Int i, j;
  j = 0;
  for (i = 0; i < DECIMAL64_Bytes; i++)
    {
      sprintf (&buf[j], "%02x", d64->bytes[i]);
      j = j + 2;
    }
  printf (" D64> %s [S:%d Cb:%02x E:%d]\n", buf,
	  decimal64Sign (d64), decimal64Comb (d64), decimal64ExpCon (d64));
}				/* decimal64Show */
#endif

/* ================================================================== */
/* Shared utility routines                                            */
/* ================================================================== */

/* define and include the conversion tables to use */
#define DEC_BIN2DPD 1		/* used for all sizes */
#if DECDPUN==3
#define DEC_DPD2BIN 1
#else
#define DEC_DPD2BCD 1
#endif
#include "decDPD.h"		/* lookup tables */

/* The maximum number of decNumberUnits we need for a working copy of */
/* the units array is the ceiling of digits/DECDPUN, where digits is */
/* the maximum number of digits in any of the formats for which this */
/* is used.  We do not want to include decimal128.h, so, as a very */
/* special case, that number is defined here. */
#define DECMAX754   34
#define DECMAXUNITS ((DECMAX754+DECDPUN-1)/DECDPUN)

/* ------------------------------------------------------------------ */
/* decDensePackCoeff -- densely pack coefficient into DPD form        */
/*                                                                    */
/*   dn is the source number (assumed valid, max DECMAX754 digits)    */
/*   bytes is the target's byte array                                 */
/*   len is length of target format's byte array                      */
/*   shift is the number of 0 digits to add on the right (normally 0) */
/*                                                                    */
/* The coefficient must be known small enough to fit, and is filled   */
/* in from the right (least significant first).  Note that the full   */
/* coefficient is copied, including the leading 'odd' digit.  This    */
/* digit is retrieved and packed into the combination field by the    */
/* caller.                                                            */
/*                                                                    */
/* shift is used for 'fold-down' padding.                             */
/*                                                                    */
/* No error is possible.                                              */
/* ------------------------------------------------------------------ */
void
decDensePackCoeff (decNumber * dn, uByte * bytes, Int len, Int shift)
{
  Int cut;			/* work */
  Int n;			/* output bunch counter */
  Int digits = dn->digits;	/* digit countdown */
  uInt dpd;			/* densely packed decimal value */
  uInt bin;			/* binary value 0-999 */
  uByte *bout;			/* -> current output byte */
  Unit *inu = dn->lsu;		/* -> current input unit */
  Unit uar[DECMAXUNITS];	/* working copy of units, iff shifted */
#if DECDPUN!=3			/* not fast path */
  Unit in;			/* current input unit */
#endif

  if (shift != 0)
    {				/* shift towards most significant required */
      /* shift the units array to the left by pad digits and copy */
      /* [this code is a special case of decShiftToMost, which could */
      /* be used instead if exposed and the array were copied first] */
      Unit *target, *source, *first;	/* work */
      uInt next = 0;		/* work */

      source = dn->lsu + D2U (digits) - 1;	/* where msu comes from */
      first = uar + D2U (digits + shift) - 1;	/* where msu will end up */
      target = uar + D2U (digits) - 1 + D2U (shift);	/* where upper part of first cut goes */

      cut = (DECDPUN - shift % DECDPUN) % DECDPUN;
      for (; source >= dn->lsu; source--, target--)
	{
	  /* split the source Unit and accumulate remainder for next */
	  uInt rem = *source % powers[cut];
	  next += *source / powers[cut];
	  if (target <= first)
	    *target = (Unit) next;	/* write to target iff valid */
	  next = rem * powers[DECDPUN - cut];	/* save remainder for next Unit */
	}
      /* propagate remainder to one below and clear the rest */
      for (; target >= uar; target--)
	{
	  *target = (Unit) next;
	  next = 0;
	}
      digits += shift;		/* add count (shift) of zeros added */
      inu = uar;		/* use units in working array */
    }

  /* densely pack the coefficient into the byte array, starting from
     the right (optionally padded) */
  bout = &bytes[len - 1];	/* rightmost result byte for phase */

#if DECDPUN!=3			/* not fast path */
  in = *inu;			/* prime */
  cut = 0;			/* at lowest digit */
  bin = 0;			/* [keep compiler quiet] */
#endif

  for (n = 0; digits > 0; n++)
    {				/* each output bunch */
#if DECDPUN==3			/* fast path, 3-at-a-time */
      bin = *inu;		/* 3 ready for convert */
      digits -= 3;		/* [may go negative] */
      inu++;			/* may need another */

#else /* must collect digit-by-digit */
      Unit dig;			/* current digit */
      Int j;			/* digit-in-bunch count */
      for (j = 0; j < 3; j++)
	{
#if DECDPUN<=4
	  Unit temp = (Unit) ((uInt) (in * 6554) >> 16);
	  dig = (Unit) (in - X10 (temp));
	  in = temp;
#else
	  dig = in % 10;
	  in = in / 10;
#endif

	  if (j == 0)
	    bin = dig;
	  else if (j == 1)
	    bin += X10 (dig);
	  else			/* j==2 */
	    bin += X100 (dig);

	  digits--;
	  if (digits == 0)
	    break;		/* [also protects *inu below] */
	  cut++;
	  if (cut == DECDPUN)
	    {
	      inu++;
	      in = *inu;
	      cut = 0;
	    }
	}
#endif
      /* here we have 3 digits in bin, or have used all input digits */

      dpd = BIN2DPD[bin];

      /* write bunch (bcd) to byte array */
      switch (n & 0x03)
	{			/* phase 0-3 */
	case 0:
	  *bout = (uByte) dpd;	/* [top 2 bits truncated] */
	  bout--;
	  *bout = (uByte) (dpd >> 8);
	  break;
	case 1:
	  *bout |= (uByte) (dpd << 2);
	  bout--;
	  *bout = (uByte) (dpd >> 6);
	  break;
	case 2:
	  *bout |= (uByte) (dpd << 4);
	  bout--;
	  *bout = (uByte) (dpd >> 4);
	  break;
	case 3:
	  *bout |= (uByte) (dpd << 6);
	  bout--;
	  *bout = (uByte) (dpd >> 2);
	  bout--;
	  break;
	}			/* switch */
    }				/* n bunches */
  return;
}				/* decDensePackCoeff */

/* ------------------------------------------------------------------ */
/* decDenseUnpackCoeff -- unpack a format's coefficient               */
/*                                                                    */
/*   byte is the source's byte array                                  */
/*   len is length of the source's byte array                         */
/*   dn is the target number, with 7, 16, or 34-digit space.          */
/*   bunches is the count of DPD groups in the decNumber (2, 5, or 11)*/
/*   odd is 1 if there is a non-zero leading 10-bit group containing  */
/*     a single digit, 0 otherwise                                    */
/*                                                                    */
/* (This routine works on a copy of the number, if necessary, where   */
/* an extra 10-bit group is prefixed to the coefficient continuation  */
/* to hold the most significant digit if the latter is non-0.)        */
/*                                                                    */
/* dn->digits is set, but not the sign or exponent.                   */
/* No error is possible [the redundant 888 codes are allowed].        */
/* ------------------------------------------------------------------ */
void
decDenseUnpackCoeff (uByte * bytes, Int len, decNumber * dn,
		     Int bunches, Int odd)
{
  uInt dpd = 0;			/* collector for 10 bits */
  Int n;			/* counter */
  uByte *bin;			/* -> current input byte */
  Unit *uout = dn->lsu;		/* -> current output unit */
  Unit out = 0;			/* accumulator */
  Int cut = 0;			/* power of ten in current unit */
  Unit *last = uout;		/* will be unit containing msd */
#if DECDPUN!=3
  uInt bcd;			/* BCD result */
  uInt nibble;			/* work */
#endif

  /* Expand the densely-packed integer, right to left */
  bin = &bytes[len - 1];	/* next input byte to use */
  for (n = 0; n < bunches + odd; n++)
    {				/* N bunches of 10 bits */
      /* assemble the 10 bits */
      switch (n & 0x03)
	{			/* phase 0-3 */
	case 0:
	  dpd = *bin;
	  bin--;
	  dpd |= (*bin & 0x03) << 8;
	  break;
	case 1:
	  dpd = (unsigned) *bin >> 2;
	  bin--;
	  dpd |= (*bin & 0x0F) << 6;
	  break;
	case 2:
	  dpd = (unsigned) *bin >> 4;
	  bin--;
	  dpd |= (*bin & 0x3F) << 4;
	  break;
	case 3:
	  dpd = (unsigned) *bin >> 6;
	  bin--;
	  dpd |= (*bin) << 2;
	  bin--;
	  break;
	}			/*switch */

#if DECDPUN==3
      if (dpd == 0)
	*uout = 0;
      else
	{
	  *uout = DPD2BIN[dpd];	/* convert 10 bits to binary 0-999 */
	  last = uout;		/* record most significant unit */
	}
      uout++;

#else /* DECDPUN!=3 */
      if (dpd == 0)
	{			/* fastpath [e.g., leading zeros] */
	  cut += 3;
	  for (; cut >= DECDPUN;)
	    {
	      cut -= DECDPUN;
	      *uout = out;
	      uout++;
	      out = 0;
	    }
	  continue;
	}
      bcd = DPD2BCD[dpd];	/* convert 10 bits to 12 bits BCD */
      /* now split the 3 BCD nibbles into bytes, and accumulate into units */
      /* If this is the last bunch and it is an odd one, we only have one */
      /* nibble to handle [extras could overflow a Unit] */
      nibble = bcd & 0x000f;
      if (nibble)
	{
	  last = uout;
	  out = (Unit) (out + nibble * powers[cut]);
	}
      cut++;
      if (cut == DECDPUN)
	{
	  *uout = out;
	  uout++;
	  cut = 0;
	  out = 0;
	}
      if (n < bunches)
	{
	  nibble = bcd & 0x00f0;
	  if (nibble)
	    {
	      nibble >>= 4;
	      last = uout;
	      out = (Unit) (out + nibble * powers[cut]);
	    }
	  cut++;
	  if (cut == DECDPUN)
	    {
	      *uout = out;
	      uout++;
	      cut = 0;
	      out = 0;
	    }
	  nibble = bcd & 0x0f00;
	  if (nibble)
	    {
	      nibble >>= 8;
	      last = uout;
	      out = (Unit) (out + nibble * powers[cut]);
	    }
	  cut++;
	  if (cut == DECDPUN)
	    {
	      *uout = out;
	      uout++;
	      cut = 0;
	      out = 0;
	    }
	}
#endif
    }				/* n */
  if (cut != 0)
    *uout = out;		/* write out final unit */

  /* here, last points to the most significant unit with digits */
  /* we need to inspect it to get final digits count */
  dn->digits = (last - dn->lsu) * DECDPUN;	/* floor of digits */
  for (cut = 0; cut < DECDPUN; cut++)
    {
      if (*last < powers[cut])
	break;
      dn->digits++;
    }
  if (dn->digits == 0)
    dn->digits++;		/* zero has one digit */
  return;
}				/*decDenseUnpackCoeff */
