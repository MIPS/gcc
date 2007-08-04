/* { dg-require-effective-target vect_int } */
#include <stdarg.h>
#include "tree-vect.h"

#define N 40
#define M 128
unsigned short in[N+M];
unsigned int out[N];

/* Outer-loop vectorization. */
/* Not vectorized due to multiple-types in the inner-loop.  */

void
foo (){
  int i,j;
  unsigned int diff;

  for (i = 0; i < N; i++) {
    diff = 0;
    for (j = 0; j < M; j+=8) {
      diff += in[j+i];
    }
    out[i]=diff;
  }
}

int main (void)
{
  int i, j;
  unsigned int diff;

  check_vect ();

  for (i = 0; i < N+M; i++) {
    in[i] = i;
  }

  foo ();

  for (i = 0; i < N; i++) {
    diff = 0;
    for (j = 0; j < M; j+=8) {
      diff += in[j+i];
    }
    if (out[i] != diff)
      abort ();
  }

  return 0;
}

/* { dg-final { scan-tree-dump-times "OUTER LOOP VECTORIZED" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump-times "vect_recog_widen_sum_pattern: not allowed" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
