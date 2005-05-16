/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* A few simple checks on arithmetic operations. */
#include <stdio.h>
extern void abort (void);

int main()
{
  /* Assumes rounding mode. */
  if (9999999.E90DF + 1.E90df != __builtin_infd32())
    abort();

  if (!__builtin_isinfd32(9999999.E90DF + 1.E90df))
    abort();

  if (9.999999999999999E384dd + .000000000000001e384dd 
      != __builtin_infd32())
    abort();

  if (-9999999.E90DF - 1.E90df != -__builtin_infd32())
    abort();

  if (!__builtin_isinfd32(9.999999999999999E384dd + .000000000000001e384dd))
    abort();

  if (7.999999999999999999999999999999999E6144dl + 3.0E6144dl
      != __builtin_infd32())
    abort();

  if (__builtin_infd32() * __builtin_infd32() != __builtin_infd32())
    abort();

  if (__builtin_infd32() * 2 != __builtin_infd32())
    abort();

  if (__builtin_infd64() * -5 != -__builtin_infd32())
    abort();

  if (!__builtin_isinfd128(__builtin_infd32() / 4))
    abort();

  if (__builtin_infd64() != __builtin_infd128())
    abort();

  if (!__builtin_isinfd64(__builtin_infd128()))
    abort();

  return 0;
}
