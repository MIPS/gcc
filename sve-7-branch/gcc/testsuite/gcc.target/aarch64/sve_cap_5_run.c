/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

#include "sve_cap_5.c"

#define COUNT 5
#define N ((COUNT + 2) * 6)

int
main (void)
{
  double x[N * 3];
  float y[N];
  for (int i = 0; i < N; ++i)
    {
      x[i * 3 + 0] = i * 2;
      x[i * 3 + 1] = i * 3;
      x[i * 3 + 2] = i * 5;
      y[i] = i * 4;
    }
  f (x, y, COUNT * 6);
  for (int i = 0; i < N; ++i)
    {
      if (i >= 6 && i < (COUNT + 1) * 6)
	{
	  if (x[i * 3 + 0] != x[i * 3 - 18] + (i - 6) * 4
	      || x[i * 3 + 1] != x[i * 3 - 17] - (i - 6) * 4
	      || x[i * 3 + 2] != x[i * 3 - 16])
	    __builtin_abort ();
	}
      else
	{
	  if (x[i * 3 + 0] != i * 2
	      || x[i * 3 + 1] != i * 3
	      || x[i * 3 + 2] != i * 5)
	    __builtin_abort ();
	}
    }
  return 0;
}
