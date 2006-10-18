#include "scalar-return-dfp_x.h"

T(d64, _Decimal64, 12.34dd)

#undef T

void
scalar_return_dfp_2_x ()
{
DEBUG_INIT

#define T(NAME) testit##NAME ();

T(d64)

DEBUG_FINI

if (fails != 0)
  abort ();

#undef T
}
