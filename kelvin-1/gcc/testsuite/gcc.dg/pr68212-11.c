/* { dg-do compile } */
/* { dg-options "-O3 -fno-tree-vectorize -funroll-loops --param  max-unroll-times=4 -fdump-rtl-loop2_unroll" } */

#include <stdio.h>

volatile int a;

extern int baz(int arg);

/*
 * loop with multiple exits and runtime iterations
 */
void foo(double *d, unsigned long int n, int a, double d1, double d2) {
  unsigned long int i;
  volatile unsigned int j;
  for (i=0;i<n;i++) {
    double dd;
    dd = d1;
    if (a > 5)
      dd = d2;
    else if (a < 0)
      break;
    else
      a = baz(a);
    d[i] =  dd;
  }
}

/* { dg-final { scan-rtl-dump-not "Invalid sum" "loop2_unroll" } } */
