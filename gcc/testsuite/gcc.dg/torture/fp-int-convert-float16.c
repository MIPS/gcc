/* Test floating-point conversions.  Standard types and _Float16.  */
/* { dg-do run } */
/* { dg-require-effective-target float16 } */
/* { dg-options "" } */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include "fp-int-convert.h"

int
main (void)
{
  TEST_I_F(signed char, unsigned char, _Float16, FLT16_MANT_DIG);
  TEST_I_F(signed short, unsigned short, _Float16, FLT16_MANT_DIG);
  TEST_I_F(signed int, unsigned int, _Float16, FLT16_MANT_DIG);
  TEST_I_F(signed long, unsigned long, _Float16, FLT16_MANT_DIG);
  TEST_I_F(signed long long, unsigned long long, _Float16, FLT16_MANT_DIG);
  exit (0);
}
