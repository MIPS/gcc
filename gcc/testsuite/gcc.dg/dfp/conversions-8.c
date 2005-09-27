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

  /*  conversion saturating.  */
  usi = 4294967296.0dd; /* { dg-warning "overflow in implicit constant conversion" } */
  if (usi !=0 )
    abort ();

  usi = 4294967296.0dl; /* { dg-warning "overflow in implicit constant conversion" } */
  if (usi !=0 )
    abort ();

  udi = 18446744073709551615.0dl;
  if (udi != 0)
    abort ();

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

  /*  conversion saturating.  */
  d64 = 4294967296.0dd;
  usi = d64;
  if (usi !=0 )
    abort ();

  d128 = 4294967296.0dl;
  usi = d128;
  if (usi !=0 )
    abort ();

  d128 = 18446744073709551615.0dl;
  udi = d128;
  if (udi != 0)
    abort ();

  return 0;
}
