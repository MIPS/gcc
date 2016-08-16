/* Test floating-point conversions.  _Float64 type with TImode.  */
/* { dg-do run } */
/* { dg-require-effective-target float64 } */
/* { dg-options "" } */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include "fp-int-convert.h"

int
main (void)
{
  TEST_I_F(TItype, UTItype, _Float64, FLT64_MANT_DIG);
  exit (0);
}
