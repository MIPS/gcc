/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_speculative_24.c"

int
main (void)
{
  int mask[] = { 1, 5, 9, 11, 15, 19, 100, 101, 102, 0xf8 };
  if (foo (mask) != 36)
    __builtin_abort ();
  return 0;
}
