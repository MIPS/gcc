/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;
volatile float sf;
volatile double df;
volatile long double tf;

extern void abort (void);

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

  d64 = 1.0000011dd;
  d32 = d64;
  if (d32 != 1.000001df)
    abort ();

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

  d128 = 1.0000011dl;
  d32 = d128;
  if (d32 != 1.000001df)
    abort ();

  d128 = 1.0000000000000011dl;
  d64 = d128;
  if (d64 != 1.000000000000001dd)
    abort ();

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

  /* 2**(-11) = 0.00048828125. */
  d128 = 0.000488281251dl;
  sf = d128;
  if (sf != 0.00048828125f)
    abort ();
  /* 2**(-25) = 0.298023223876953125E-7.  */
  d128 = 2.98023223876953125E-8dl;
  df = d128;
  if (df < (2.9802322387695312e-08 - 0.00000000001)
      || df > (2.9802322387695312e-08 + 0.00000000001))
    abort ();

  /* Test demotion to non-representable decimal floating type. */
  /* Assumes a default rounding mode of 'near'.  The rules are a 
     bit odd in that if off by one digit, it rounds to the maximum
     value, but otherwise to HUGE_VAL. */

  /* FIXME: Add some tests here. See conversions-4.c. */

  return 0;
}
