/* { dg-do run { target powerpc*-*-* } } */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -msse2" { target i?86-*-* x86_64-*-* } } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 17

float b[N] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,31};
float a[N];

int main1 ()
{
  int i;

  /* Vectorized: known loop bound
     non-divisible by vectorization factor.  */
  for (i = 0; i < N; i++) {
    a[i] = b[i];
  }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (a[i] != b[i])
        abort ();
    }

  return 0;
}

int main (void)
{ 
  check_vect ();
  
  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
