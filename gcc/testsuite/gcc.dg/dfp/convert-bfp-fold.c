/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

extern void abort (void);

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;
float sf;
double df;
long double tf;

int main()
{
  /* Conversions between decimal floats. */

  /* Conversions from _Decimal32. */
  sf = 2.0df;
  if (sf != 2.0)
    abort();

  df = 2.0df;
  if (df != 2.0)
    abort();

  tf = 2.0df;
  if (tf != 2.0)
    abort();

  /* Conversions from _Decimal64. */
  sf = -7.0dd;
  if (sf != -7.0)
    abort();
  
  df = -7.0dd;
  if (df != -7.0)
    abort();

  tf = -7.0dd;
  if (tf != -7.0)
    abort();

  /* Conversions from _Decimal128. */
  sf = 30.0dl;
  if (sf != 30.0)
    abort();

  df = 3.0dl;
  if (df != 3.0)
    abort();

  df = 3.0dl;
  if (df != 3.0)
    abort();

  /* Conversions to regular floating point. */
  d32 = 30.0f;
  if (d32 != 30.0df)
    abort();

  d64 = 30.0f;
  if (d64 != 30.0dd)
    abort();

  d128 = 30.0f;
  if (d128 != 30.0dl)
    abort();

  d32 = -2.0;
  if (d32 != -2.0df)
    abort();

  d64 = -2.0;
  if (d64 != -2.0dd)
    abort();

  d128 = -2.0;
  if (d128 != -2.0dl)
    abort();
  
  d32 = -22.0l;
  if (d32 != -22.0df)
    abort();

  d64 = -22.0l;
  if (d64 != -22.0dd)
    abort();

  d128 = -22.0l;
  if (d128 != -22.0dl)
    abort();


  /* Test demotion to non-representable decimal floating type. */
  /* Assumes a default rounding mode of 'near'.  The rules are a 
     bit odd in that if off by one digit, it rounds to the maximum
     value, but otherwise to HUGE_VAL. */

  /* FIXME: Add some tests here. See conversions-4.c. */

  return 0;
}
