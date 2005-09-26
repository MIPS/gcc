/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

extern void abort (void);

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;
unsigned int usi;
unsigned long long udi;

int main()
{
  /* Constant conversions to unsigned integers. */
  usi = 1.50df;

  if (usi != 1)
    abort();

  udi = 1.50df;
  if (udi != 1)
    abort();

  usi = 1.55dd;
  if (usi != 1)
    abort();

  udi = 1.55dd;
  if (udi != 1)
    abort();

  usi = 1555.55dl;
  if (usi != 1555)
    abort();

  udi = 1555.55dl;
  if (udi != 1555)
    abort();

  /* Conversions from unsigned integer constants.*/
  d32 = 1555;
  if (d32 != 1555.0df)
    abort();

  d32 = 1555LL;
  if (d32 != 1555.0df)
    abort();

  d64 = 1555;
  if (d64 != 1555.0dd)
    abort();

  d64 = 1555LL;
  if (d64 != 1555.0dd)
    abort();

  d128 = 1555;
  if (d128 != 1555.0dl)
    abort();

  d128 = 1555LL;
  if (d128 != 1555.0dl)
    abort();

  return 0;
}
