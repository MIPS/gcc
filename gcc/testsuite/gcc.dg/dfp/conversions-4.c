/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

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
  /* Assumes a default rounding mode of 'near'.  This uses the rules
     describe in the 27 July 2005 draft of IEEE 754r, which are much
     more clear that what's described in draft 5 of N1107.  */

  /* Rounds to what _Decimal32 can handle.  */
  d64 = 9.99999949E96DD;
  d32 = d64;
  if (d32 != __DEC32_MAX__)
    abort();

  /* Rounds to more than _Decimal32 can handle.  */
  d64 = 9.9999995E96DD;
  d32 = d64;
  if (d32 != __builtin_infd32())
    abort();

  /* Rounds to what _Decimal32 can handle.  */
  d128 = 9.99999949E96DD;
  d32 = d128;
  if (d32 != __DEC32_MAX__)
    abort();

  /* Rounds to more than _Decimal32 can handle.  */
  d128= 9.9999995E96DD;
  d32 = d128;
  if (d32 != __builtin_infd32())
    abort();

  /* Rounds to what _Decimal64 can handle.  */
  d128 = 9.99999999999999949E384DL;
  d64 = d128;
  if (d64 != __DEC64_MAX__)
    abort();

  /* Rounds to more than _Decimal64 can handle.  */
  d128 = 9.9999999999999995E384DL;
  d64 = d128;
  if (d64 != __builtin_infd64())
    abort();

  return 0;
}
