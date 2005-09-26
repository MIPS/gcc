/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

extern void abort (void);

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;
volatile signed int si;
volatile signed long long di;

int main()
{

  /* Conversions from signed integers. */
  d32 = -1.50df;
  si = d32;

  if (si != -1)
    abort();

  di = d32;
  if (di != -1)
    abort();

  d64 = -1.55dd;
  si = d64;
  if (si != -1)
    abort();

  di = d64;
  if (di != -1)
    abort();

  d128 = -1555.55dl;
  si = d128;
  if (si != -1555)
    abort();

  di = d128;
  if (di != -1555)
    abort();

  /* Conversions to signed integers. */
  d32 = si;
  if (d32 != -1555.0df)
    abort();

  d32 = di;
  if (d32 != -1555.0df)
    abort();

  d64 = si;
  if (d64 != -1555.0dd)
    abort();

  d64 = di;
  if (d64 != -1555.0dd)
    abort();

  d128 = si;
  if (d128 != -1555.0dl)
    abort();

  d128 = di;
  if (d128 != -1555.0dl)
    abort();

  return 0;
}
