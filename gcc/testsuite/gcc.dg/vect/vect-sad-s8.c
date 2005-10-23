/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"
#include <stdio.h>

#define N 64
#define SAD16 3152
#define SAD32 3152

/* Vectorizable using {SAD_EXPR}, or using
   {SUB_EXPR, ABS_EXPR, WIDEN_SUM_EXPR}, or using
   {SUB_EXPR, ABS_EXPR, UNPACK_HI/LO_EXPR, PLUS_EXPR}.  */ 

short sad16 (short sad,
             int n,
             signed char *in1,
             signed char *in2)
{
  signed char absdiff;
  int i;

  for (i = 0; i < n; i++) {
    absdiff = in1[i] - in2[i];
    if (absdiff < 0)
      absdiff = 0 - absdiff;
    sad += absdiff;
  }

  return sad;
}

int sad32 (int sad,
           int n,
           signed char *in1,
           signed char *in2)
{
  signed char absdiff;
  int i;

  for (i = 0; i < n; i++) {
    absdiff = in1[i] - in2[i];
    if (absdiff < 0)
      absdiff = 0 - absdiff;
    sad += absdiff;
  }

  return sad;
}

int main (void)
{
  int i;
  int sad_32=0;
  short sad_16=0;
  signed char in1[N], in2[N];

  check_vect ();

  for (i=0; i<N; i++) {
    in2[i] = 16-i;
    in1[i] = i;
  }

  sad_16 = sad16 (sad_16, N, in1, in2);
  sad_32 = sad32 (sad_32, N, in1, in2);

  /* Check results */
  if (sad_16 != SAD16 || sad_32 != SAD32)
    abort();

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

