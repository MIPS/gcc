/* Test floating-point conversions.  Standard types and _Float32x.  */
/* { dg-do run } */
/* { dg-require-effective-target float32x } */
/* { dg-options "" } */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include "fp-int-convert.h"

int
main (void)
{
  TEST_I_F(signed char, unsigned char, _Float32x, FLT32X_MANT_DIG);
  TEST_I_F(signed short, unsigned short, _Float32x, FLT32X_MANT_DIG);
  TEST_I_F(signed int, unsigned int, _Float32x, FLT32X_MANT_DIG);
  TEST_I_F(signed long, unsigned long, _Float32x, FLT32X_MANT_DIG);
  TEST_I_F(signed long long, unsigned long long, _Float32x, FLT32X_MANT_DIG);
  exit (0);
}
