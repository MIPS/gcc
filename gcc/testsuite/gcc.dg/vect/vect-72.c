/* { dg-do run { target powerpc*-*-* } } */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -msse" { target i?86-*-* x86_64-*-* } } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 128

/* unaligned load.  */

int main1 (int n)
{
  int i;
  char ia[N];
  char ib[N+1];

  for (i=0; i < n+1; i++)
    {
      ib[i] = i;
    }

  for (i = 1; i < n+1; i++)
    {
      ia[i-1] = ib[i];
    }

  /* check results:  */
  for (i = 1; i <= n; i++)
    {
      if (ia[i-1] != ib[i])
        abort ();
    }

  return 0;
}

int main (void)
{ 
  check_vect ();
  
  return main1 (N);
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */

