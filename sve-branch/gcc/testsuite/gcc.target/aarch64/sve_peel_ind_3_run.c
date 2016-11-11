/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_peel_ind_3.c"

volatile int y;

int
main (void)
{
  for (int start = 0; start < MAX_START; ++start)
    {
      foo (start);
      for (int i = 0; i < N; ++i)
	{
	  if (x[start][i] != (i < start || i >= start + COUNT ? 0 : i))
	    __builtin_abort ();
	  y++;
	}
    }
  return 0;
}
