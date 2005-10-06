/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 64

#define DOT1 43680
#define DOT2 360096

signed short X[N] __attribute__ ((__aligned__(16)));
signed short Y[N] __attribute__ ((__aligned__(16)));
unsigned char CX[N] __attribute__ ((__aligned__(16)));
unsigned char CY[N] __attribute__ ((__aligned__(16)));

void
foo1(int len) {
  int i;
  int result1 = 0;
  unsigned int result2 = 0;
  unsigned short prod;

  for (i=0; i<len; i++) {
    result1 += (X[i] * Y[i]);
    prod = CX[i] * CY[i];
    result2 += prod;
  }

  if (result1 != DOT1 || result2 != DOT2)
    abort ();
}


int main (void)
{
  int i, dot1, dot2;

  check_vect ();

  for (i=0; i<N; i++) {
    X[i] = i;
    Y[i] = 64-i;
    CX[i] = i;
    CY[i] = 32-i;
  }

  foo1 (N);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

