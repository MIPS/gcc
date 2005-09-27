/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

extern void abort (void);

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;
volatile unsigned int usi;
volatile unsigned long long udi;

int main()
{
  /* Conversions to unsigned integers. */
  d32 = 1.50df;
  usi = d32;

  if (usi != 1)
    abort();

  udi = d32;
  if (udi != 1)
    abort();

  d64 = 1.55dd;
  usi = d64;
  if (usi != 1)
    abort();

  udi = d64;
  if (udi != 1)
    abort();

  d128 = 1555.55dl;
  usi = d128;
  if (usi != 1555)
    abort();

  udi = d128;
  if (udi != 1555)
    abort();

  /* Conversions from unsigned integers. */
  d32 = usi;
  if (d32 != 1555.0df)
    abort();

  d32 = udi;
  if (d32 != 1555.0df)
    abort();

  d64 = usi;
  if (d64 != 1555.0dd)
    abort();

  d64 = udi;
  if (d64 != 1555.0dd)
    abort();

  d128 = usi;
  if (d128 != 1555.0dl)
    abort();

  d128 = udi;
  if (d128 != 1555.0dl)
    abort();

  return 0;
}
