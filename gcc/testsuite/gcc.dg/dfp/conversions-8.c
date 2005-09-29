/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test various conversions involving decimal floating types. */

#include <limits.h>

extern void abort (void);

_Decimal32 d32;
_Decimal64 d64;
_Decimal128 d128;

signed int si;
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

  /* Test integer saturation.  */

  /* Unsigned.  */
  usi = __DEC32_MAX__;  /* { dg-warning "overflow in implicit constant conversion" } */
  if (usi != UINT_MAX)
    abort ();

  usi = __DEC64_MAX__;  /* { dg-warning "overflow in implicit constant conversion" } */
  if (usi != UINT_MAX)
    abort ();

  usi = __DEC128_MAX__; /* { dg-warning "overflow in implicit constant conversion" } */
  if (usi != UINT_MAX)
    abort ();

  /* Signed.  */
  si = __DEC32_MAX__;	/* { dg-warning "overflow in implicit constant conversion" } */
  if (si != INT_MAX)
    abort ();

  si = __DEC64_MAX__;   /* { dg-warning "overflow in implicit constant conversion" } */
  if (si != INT_MAX)
    abort ();

  si = __DEC128_MAX__;  /* { dg-warning "overflow in implicit constant conversion" } */
  if (si != INT_MAX)
    abort ();

  si = - __DEC32_MAX__; /* { dg-warning "overflow in implicit constant conversion" } */
  if (si != INT_MIN)
    abort ();

  si = - __DEC64_MAX__; /* { dg-warning "overflow in implicit constant conversion" } */
  if (si != INT_MIN)
    abort ();

  si = - __DEC128_MAX__; /* { dg-warning "overflow in implicit constant conversion" } */
  if (si != INT_MIN)
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

  /* We cannot easily test DFP saturation because we don't have
     integer types big enough!  The smallest decimal floating point
     type (_Decimal32) has the range [E-96 to E+96] and we would need
     around 300-bit wide integers to exceed that range.  */

  /*
    d32 = ULONG_MAX;
    if (! __builtin_isnand32 (d32))
      abort ();
  
    d64 = ULONG_MAX;
    if (! __builtin_isnand64 (d64))
      abort ();
    
    d128 = ULONG_MAX;
    if (! __builtin_isnand128 (d128))
      abort ();
  */
    
  return 0;
}
