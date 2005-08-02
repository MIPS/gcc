/* { dg-do run } */
/* { dg-options "-std=gnu99 -O3" } */

/* Test various conversions involving decimal floating types. */
#include <stdio.h>
#include <stdlib.h>

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;

int main()
{
  /* Conversions between decimal floats.  Similar to conversions-4.c
     but most of the checks below should be optimized away, exercising
     different code-paths in GCC. */
   
  d64 = 1.5df;
  if (d64 != 1.5dd)
    abort();

  d128 = 1.5df;
  if (d128 != 1.5dl)
    abort();

  /* Conversions from _Decimal64. */
  d32 = -1.5dd;
  if (d32 != -1.5df)
    abort();
  
  d128 = -1.5dd;
  if (d128 != -1.5dl)
    abort();

  /* Conversions from _Decimal128. */
  d32 = 2.15dl;
  if (d32 != 2.15df)
    abort();

  d64 = 2.15dl;
  if (d64 != 2.15dd)
    abort();

  /* Test demotion to non-representable decimal floating type. */
  /* Assumes a default rounding mode of 'near'.  This uses the rules
     describe in the 27 July 2005 draft of IEEE 754r, which are much
     more clear that what's described in draft 5 of N1107.  */
  d32 = 9.99999949E96DD;
  if (d32 != __DEC32_MAX__)
    abort();

  d32 = 9.9999995E96DD;
  if (d32 != __builtin_infd32 ())
    abort();

  /* Rounds to what the type can handle.  */
  d64 = 9.99999999999999949E384DL;
  if (d64 != __DEC64_MAX__)
    abort();

  /* Rounds to more than the type can handle.  */
  d64 = 9.9999999999999995E384DL;
  if (d64 != __builtin_infd64())
    abort();

  return 0;
}
