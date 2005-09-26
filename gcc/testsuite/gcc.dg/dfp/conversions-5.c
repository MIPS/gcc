/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */
#include <stdio.h>
#include <stdlib.h>

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;
volatile float sf;
volatile double df;
volatile long double tf;

int main()
{
  /* Conversions from decimal float to binary float. */

  /* Conversions from _Decimal32. */
  d32 = 2.0df;
  sf = d32;
  if (sf != 2.0)
    abort();

  df = d32;
  if (df != 2.0)
    abort();

  tf = d32;
  if (tf != 2.0)
    abort();

  /* Conversions from _Decimal64. */
  d64 = -7.0dd;
  sf = d64;
  if (sf != -7.0)
    abort();
  
  df = d64;
  if (df != -7.0)
    abort();

  tf = d64;
  if (tf != -7.0)
    abort();

  /* Conversions from _Decimal128. */
  d128 = 30.0dl;
  sf = d128;
  if (sf != 30.0)
    abort();

  df = d128;
  if (df != 30.0)
    abort();

  df = d128;
  if (df != 30.0)
    abort();

  /* Conversions from binary float to decimal float. */
  sf = 30.0f;
  d32 = sf;
  if (d32 != 30.0df)
    abort();

  d64 = sf;
  if (d64 != 30.0dd)
    abort();

  df = -2.0;
  d32 = df;
  if (d32 != -2.0df)
    abort();

  d64 = df;
  if (d64 != -2.0dd)
    abort();

  d128 = df;
  if (d128 != -2.0dl)
    abort();
  
  d128 = sf;
  if (d128 != 30.0dl)
    abort();

  tf = -22.0l;
  d32 = tf;
  if (d32 != -22.0df)
    abort();

  d64 = tf;
  if (d64 != -22.0dd)
    abort();

  d128 = tf;
  if (d128 != -22.0dl)
    abort();

  /* Test demotion to non-representable decimal floating type. */
  /* Assumes a default rounding mode of 'near'.  The rules are a 
     bit odd in that if off by one digit, it rounds to the maximum
     value, but otherwise to HUGE_VAL. */

  /* FIXME: Add some tests here. See conversions-4.c. */

  return 0;
}
