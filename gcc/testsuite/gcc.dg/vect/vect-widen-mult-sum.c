/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 64
#define SUM -3
 
/* Require widening-mult or data-unpacking (for the type promotion).  */
int
main1 (short *in, int off, short scale, int n)
{
 int i;
 int sum = 0;

 for (i = 0; i < n; i++) {
   sum += ((int) in[i] * (int) in[i+off]) >> scale;
 }

 return sum;
}

int main (void)
{
  int i;
  int sum;
  short X[N];

  check_vect ();

  for (i=0; i<N; i++) {
    X[i] = 16-i; 
  }

  sum = main1 (X, 1, 16, N);

  if (sum != SUM)
    abort ();

  return 0;
}


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
