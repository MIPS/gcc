/* { dg-do run { target powerpc*-*-* } } */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-O2 -ftree-vectorize -fdump-tree-vect-stats -msse2" { target i?86-*-* x86_64-*-* } } */

#include <stdarg.h>
#include "tree-vect.h"

typedef char achar __attribute__ ((__aligned__(16)));

#define N 16
achar x[N];
 
int main1 (achar *y)
{  
  struct {
    achar *p;
  } s;
  achar cb[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
  int i;

  s.p = y;
  for (i = 0; i < N; i++)
    {
      s.p[i] = cb[i];
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (s.p[i] != cb[i])
        abort ();
    }

  return 0;
}

int main2 ()
{
  struct {
    achar *p;
  } s;
  achar cb[N] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45};
  int i;

  s.p = x;
  for (i = 0; i < N; i++)
    {
      s.p[i] = cb[i];
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (s.p[i] != cb[i])
        abort ();
    }

  return 0;
}


int main (void)
{ 
  check_vect ();
  
  main1 (x);
  main2 ();
  return 0;
} 


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" { xfail *-*-* } } } */
