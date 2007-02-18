/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include <stdio.h>
#include "tree-vect.h"

#define N 8

typedef struct {
   unsigned short a;
   unsigned short b;
} s;

typedef struct {
   unsigned int a;
   unsigned int b;
} ii;

int
main1 (s *arr, ii *iarr)
{
  s *ptr = arr;
  ii *iptr = iarr;
  s res[N];
  ii ires[N];
  int i;
  unsigned int in[N] = {0,1,2,3,4,5,6,7};
  unsigned int inin[N] = {16,17,18,19,20,21,22,23};
  unsigned int out[N*2] = {16,16,16,18,16,20,16,22,16,24,16,26,16,28,16,30};

  for (i = 0; i < N; i++)
    {
      ires[i].a = inin[i] - in[i];
      ires[i].b = in[i] + inin[i];
      res[i].b = 2;
      res[i].a = 3;
      iptr++;
      ptr++;
    }

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (res[i].b != 2
          || ires[i].a != out[2*i] 
          || res[i].a != 3 
          || ires[i].b != out[2*i + 1])
        abort ();
    }

  return 0;
}

int main (void)
{
  int i;
  s arr[N];
  ii iarr[N];

  check_vect ();

  for (i = 0; i < N; i++)
    {
      arr[i].a = i;
      arr[i].b = i * 2;
      iarr[i].a = i;
      iarr[i].b = i * 3;
      if (arr[i].a == 178)
         abort();
    }

  main1 (arr, iarr); 
  
  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { target vect_interleave } } } */
/* { dg-final { scan-tree-dump-times "Constant stores in interleaving" 1 "vect"  } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

