/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_peel_ind_1.c"

volatile int y;

int
main (void)
{
  foo ();
  for (int i = 0; i < N; ++i)
    {
      if (x[i] != (i < START || i >= END ? 0 : (i - START) * 5))
	__builtin_abort ();
      y++;
    }
  return 0;
}
