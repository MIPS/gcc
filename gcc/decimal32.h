/* ------------------------------------------------------------------ */
/* Decimal 32-bit format module header                                */
/* ------------------------------------------------------------------ */
/* Copyright (c) IBM Corporation, 2000, 2003.  All rights reserved.   */
/*                                                                    */
/* This software is made available under the terms of the IBM         */
/* alphaWorks License Agreement (distributed with this software as    */
/* alphaWorks-License.txt).  Your use of this software indicates      */
/* your acceptance of the terms and conditions of that Agreement.     */
/*                                                                    */
/* The description and User's Guide ("The decNumber C Library") for   */
/* this software is included in the package as decNumber.pdf.  This   */
/* document is also available in HTML, together with specifications,  */
/* testcases, and Web links, at: http://www2.hursley.ibm.com/decimal  */
/*                                                                    */
/* Please send comments, suggestions, and corrections to the author:  */
/*   mfc@uk.ibm.com                                                   */
/*   Mike Cowlishaw, IBM Fellow                                       */
/*   IBM UK, PO Box 31, Birmingham Road, Warwick CV34 5JL, UK         */
/* ------------------------------------------------------------------ */

#if !defined(DECIMAL32)
  #define DECIMAL32
  #define DEC32NAME     "decimal32"                   /* Short name */
  #define DEC32FULLNAME "Decimal 32-bit Number"       /* Verbose name */
  #define DEC32AUTHOR   "Mike Cowlishaw"              /* Who to blame */

  /* parameters for decimal32s */
  #define DECIMAL32_Bytes  4            // length
  #define DECIMAL32_Pmax   7            // maximum precision (digits)
  #define DECIMAL32_Emax   96           // maximum adjusted exponent
  #define DECIMAL32_Emin  -95           // minimum adjusted exponent
  #define DECIMAL32_Bias   101          // bias for the exponent
  #define DECIMAL32_String 15           // maximum string length, +1
  // highest biased exponent (Elimit-1)
  #define DECIMAL32_Ehigh  (DECIMAL32_Emax+DECIMAL32_Bias-DECIMAL32_Pmax+1)

  #ifndef DECNUMDIGITS
    #define DECNUMDIGITS DECIMAL32_Pmax // size if not already defined
  #endif
  #ifndef DECNUMBER
    #include "decNumber.h"              // context and number library
  #endif

  /* Decimal 32-bit type, accessible by bytes */
  typedef struct {
    uint8_t bytes[DECIMAL32_Bytes];     // decimal32: 1, 5, 6, 20 bits
    } decimal32;

  /* special values [top byte excluding sign bit; last two bits are
     don't-care for Infinity on input, last bit don't-care for NaN] */
  #if !defined(DECIMAL_NaN)
    #define DECIMAL_NaN     0x7c        // 0 11111 00 NaN
    #define DECIMAL_sNaN    0x7e        // 0 11111 10 sNaN
    #define DECIMAL_Inf     0x78        // 0 11110 00 Infinity
  #endif

  /* Macros for accessing decimal32 fields.  These assume the argument
     is a reference (pointer) to the decimal32 structure */
  // Get sign
  #define decimal32Sign(d)       ((unsigned)(d)->bytes[0]>>7)

  // Get combination field
  #define decimal32Comb(d)       (((d)->bytes[0] & 0x7c)>>2)

  // Get exponent continuation [does not remove bias]
  #define decimal32ExpCon(d)     ((((d)->bytes[0] & 0x03)<<4)         \
                               | ((unsigned)(d)->bytes[1]>>4))

  // Set sign [this assumes sign previously 0]
  #define decimal32SetSign(d, b) {                                    \
    (d)->bytes[0]|=((unsigned)(b)<<7);}

  // Set exponent continuation [does not apply bias]
  // This assumes range has been checked and exponent previously 0;
  // type of exponent must be unsigned
  #define decimal32SetExpCon(d, e) {                                  \
    (d)->bytes[0]|=(uint8_t)((e)>>4);                                 \
    (d)->bytes[1]|=(uint8_t)(((e)&0x0F)<<4);}

  /* ------------------------------------------------------------------ */
  /* Routines                                                           */
  /* ------------------------------------------------------------------ */
  // String conversions
  decimal32 * decimal32FromString(decimal32 *, char *, decContext *);
  char * decimal32ToString(decimal32 *, char *);
  char * decimal32ToEngString(decimal32 *, char *);

  // decNumber conversions
  decimal32 * decimal32FromNumber(decimal32 *, decNumber *, decContext *);
  decNumber * decimal32ToNumber(decimal32 *, decNumber *);

#endif
