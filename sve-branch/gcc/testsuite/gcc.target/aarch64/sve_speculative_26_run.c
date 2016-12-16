/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256" } */

#include "sve_speculative_26.c"

int
main (void)
{
  if (foo () != 45)
    __builtin_abort ();
  return 0;
}
