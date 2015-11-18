/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -funroll-loops --param  max-unroll-times=4 -fdump-rtl-loop2_unroll" } */

#include <stdio.h>

volatile int a;

/* if-then-else statement embedded within a constant iterations loop,
   where constant number of iterations is not divisible by 4. */
void foo(double *d, unsigned long int n, double d1, double d2)
{
  unsigned long int i;
  volatile unsigned int j;
  for (i=0;i<1000001;i++)
    {
      double dd;
      dd = d1;
      if (a > 5)
	dd = d2;
      d[i] =  dd;
    }
}

/* { dg-final { scan-rtl-dump-not "Invalid sum" "loop2_unroll" } } */
