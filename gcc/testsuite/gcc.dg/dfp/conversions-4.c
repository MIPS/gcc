/* Make the test compile-only until runtime conversions are supported.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */
#include <stdio.h>
extern void abort (void);

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;

int main()
{
  /* Conversions between decimal floats. */

  /* Conversions from _Decimal32. */
  d32 = 1.5df;
  d64 = d32;
  if (d64 != 1.5dd)
    abort();

  d128 = d32;
  if (d128 != 1.5dl)
    abort();

  /* Conversions from _Decimal64. */
  d64 = -1.5dd;
  d32 = d64;
  if (d32 != -1.5df)
    abort();
  
  d128 = d64;
  if (d128 != -1.5dl)
    abort();

  /* Conversions from _Decimal128. */
  d128 = 2.15dl;
  d32 = d128;
  if (d32 != 2.15df)
    abort();

  d64 = d128;
  if (d64 != 2.15dd)
    abort();

  /* Test demotion to non-representable decimal floating type. */
  /* Assumes a default rounding mode of 'near'.  The rules are a 
     bit odd in that if off by one digit, it rounds to the maximum
     value, but otherwise to HUGE_VAL. */
  d64 = 9.9999991E96DD;
  d32 = d64;
  if (d32 != __DEC32_MAX__)
    abort();

  d64 = 9.99999912E96DD;
  d32 = d64;
  if (d32 != __builtin_infd32())
    abort();

  /* One digit more than _Decimal32 can handle. */
  d128 = 9.9999991E96DD;
  d32 = d128;
  if (d32 != __DEC32_MAX__)
    abort();

  /* Two digits more than _Decimal32 can handle. */
  d128= 9.99999912E96DD;
  d32 = d128;
  if (d32 != __builtin_infd32())
    abort();

  /* One digit more that _Decimal64 can handle. */
  d128 = 9.9999999999999991E384DL;
  d64 = d128;
  if (d64 != __DEC64_MAX__)
    abort();

  /* Two digits more than _Decimal64 can handle. */
  d128 = 9.99999999999999912E384DL;
  d64 = d128;
  if (d64 != __builtin_infd64())
    abort();

  return 0;
}
