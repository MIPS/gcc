/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Test integer saturation.  */

#include <decfloat.h>
#include <limits.h>

extern void abort (void);

volatile _Decimal32 d32;
volatile _Decimal64 d64;
volatile _Decimal128 d128;

volatile signed int si;
volatile unsigned int usi;
volatile unsigned long long udi;

int
main ()
{

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

  return 0;
}
