/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -funroll-loops --param  max-unroll-times=4 -fdump-rtl-loop2_unroll" } */

void foo(double *d, unsigned long int n) {
  unsigned long int i;
  volatile unsigned int j;
  for (i=0;i<1000000;i++)
      d[j] =  0.0;
}

/* { dg-final { scan-rtl-dump-not "Invalid sum" "loop2_unroll" } } */
