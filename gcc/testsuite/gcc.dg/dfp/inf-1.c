/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* A few simple checks on arithmetic operations. */
#include <stdio.h>
extern void abort (void);

int main()
{
  /* Assumes rounding mode. */
  if (9999999.E90DF + 1.E90df != __builtin_infdf())
    abort();

  if (!__builtin_isinfdf(9999999.E90DF + 1.E90df))
    abort();

  if (9.999999999999999E384dd + .000000000000001e384dd 
      != __builtin_infdf())
    abort();

  if (-9999999.E90DF - 1.E90df != -__builtin_infdf())
    abort();

  if (!__builtin_isinfdf(9.999999999999999E384dd + .000000000000001e384dd))
    abort();

  if (7.999999999999999999999999999999999E6144dl + 3.0E6144dl
      != __builtin_infdf())
    abort();

  if (__builtin_infdf() * __builtin_infdf() != __builtin_infdf())
    abort();

  if (__builtin_infdf() * 2 != __builtin_infdf())
    abort();

  if (__builtin_infdd() * -5 != -__builtin_infdf())
    abort();

  if (!__builtin_isinfdl(__builtin_infdf() / 4))
    abort();

  if (__builtin_infdd() != __builtin_infdl())
    abort();

  if (!__builtin_isinfdd(__builtin_infdl()))
    abort();

  return 0;
}
