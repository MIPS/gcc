/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_speculative_29.c"

int
main (void)
{
  if (f (4, 2.0f) != 4.0f)
    __builtin_abort ();
  if (f (32, 4.0f) != 20.0f)
    __builtin_abort ();
  if (f (128, 100.f) != 700.0f)
    __builtin_abort ();
  if (f (16384, 3.0f) != 42.0f)
    __builtin_abort ();
  return 0;
}
