/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_speculative_28.c"

int
main (void)
{
  if (f (4) != 2)
    __builtin_abort ();
  if (f (32) != 5)
    __builtin_abort ();
  if (f (128) != 7)
    __builtin_abort ();
  if (f (16384) != 14)
    __builtin_abort ();
  return 0;
}
