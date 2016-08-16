/* Test floating-point conversions.  Standard types and _Float64.  */
/* { dg-do run } */
/* { dg-require-effective-target float64 } */
/* { dg-options "" } */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include "fp-int-convert.h"

int
main (void)
{
  TEST_I_F(signed char, unsigned char, _Float64, FLT64_MANT_DIG);
  TEST_I_F(signed short, unsigned short, _Float64, FLT64_MANT_DIG);
  TEST_I_F(signed int, unsigned int, _Float64, FLT64_MANT_DIG);
  TEST_I_F(signed long, unsigned long, _Float64, FLT64_MANT_DIG);
  TEST_I_F(signed long long, unsigned long long, _Float64, FLT64_MANT_DIG);
  exit (0);
}
