/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 64

unsigned char X[N] __attribute__ ((__aligned__(16)));
unsigned short result[N];

int
foo1(int len) {
  int i;

  for (i=0; i<len; i++) {
    result[i] = (unsigned short)X[i];
  }
}

int main (void)
{
  int i;

  check_vect ();

  for (i=0; i<N; i++) {
    X[i] = i;
  }

  foo1 (N);

  for (i=0; i<N; i++) {
    if (result[i] != (unsigned short)X[i])
      abort ();
  }
  
  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

