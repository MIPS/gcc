/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -funroll-loops --param  max-unroll-times=4 -fdump-rtl-loop2_unroll" } */

volatile int a;

void foo(double *d, unsigned long int n, double d1, double d2)
{
  unsigned long int i;

  for (i=0;i<n;i++)
    {
      double dd;
      dd = d1;
      if (a > 5)
	dd = d2;
      d[i] =  dd;
    }
}

/* { dg-final { scan-rtl-dump-not "Invalid sum" "loop2_unroll" } } */
