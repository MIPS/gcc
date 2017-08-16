/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-Ofast -march=armv8-a+sve -mtune=thunderx -fno-vect-cost-model" } */
/* { dg-options "-Ofast -march=armv8-a+sve -msve-vector-bits=256 -mtune=thunderx -fno-vect-cost-model" { target aarch64_sve256_hw } } */

#include "sve_peel_ind_4.c"

volatile int y;

int
main (void)
{
  double x[END + 1];
  for (int i = 0; i < END + 1; ++i)
    x[i] = i;
  foo (x);
  for (int i = 0; i < END + 1; ++i)
    {
      double expected;
      if (i < START || i >= END)
	expected = i;
      else
	expected = 10 + (i - START) * 5;
      if (x[i] != expected)
	__builtin_abort ();
      y++;
    }
  return 0;
}
