/* { dg-require-effective-target vect_float } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 40
#define M 128
float in[N+M];
float coeff[M];
float out[N];
float fir_out[N];


void foo (){
 int i,j,k;
 float diff;

 for (i = 0; i < N; i++) {
  out[i] = 0;
 }

 for (k = 0; k < 4; k++) {
  for (i = 0; i < N; i++) {
    diff = 0;
    for (j = k; j < M; j+=4) {
      diff += in[j+i]*coeff[j-k]; 
    }
    out[i] += diff;
  }
 }

}


void fir (){
  int i,j,k;
  float diff;

  for (i = 0; i < N; i++) {
    diff = 0;
    for (j = 0; j < M; j++) {
      diff += in[j+i]*coeff[j];
    }
    fir_out[i] = diff;
  }
}

int main (void)
{
  check_vect ();
  int i, j;
  float diff;

  foo ();
  fir ();
  
  for (i = 0; i < N; i++) {
    if (out[i] != fir_out[i])
      abort ();
  }

  return 0;
}


/* { dg-final { scan-tree-dump-times "OUTER LOOP VECTORIZED" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
