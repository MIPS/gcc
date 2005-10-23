/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 64
#define RES1 2024976
#define RES2 122976
 
/* Vectorization of out[i] requires
   mult_highpart, or
   widening-mult and data-packing, or 
   data-unpacking, regular multiplication, and data packing.

   Vectorization of result requires
   dot_product, or
   widening-mult and summation, or
   data-unpacking, regular multiplication, and summation.  */
int
main1 (short * __restrict__ in, short * __restrict__ out, short fade, int n)
{
 int i;
 int tmp;
 int result = 0;

 for (i = 0; i < n; i++) {
   tmp = in[i] * fade;
   out[i] = tmp >> 16;
   result += tmp;
 }

 return result;
}

int
main2 (unsigned char * __restrict__ in, unsigned char * __restrict__ out, 
       unsigned char fade, int n)
{
 int i;
 unsigned short tmp;
 unsigned int result=0;

 for (i = 0; i < n; i++) {
   tmp = in[i] * fade;
   out[i] = tmp >> 8;
   result += tmp;
 }

 return result;
}

int main (void)
{
  int i;
  int sum;
  short X_in[N], X_out[N];
  unsigned char Y_in[N], Y_out[N];
  int res1;
  unsigned int res2;

  check_vect ();

  for (i=0; i<N; i++) {
    X_in[i] = 500+i; 
    Y_in[i] = i;
  }

  res1 = main1 (X_in, X_out, 61, N);
  res2 = main2 (Y_in, Y_out, 61, N);

  /* Check results */
  for (i=0; i<N; i++)
    {
      int tmpX = X_in[i] * (short)61;
      unsigned short tmpY = Y_in[i] * (unsigned char)61;
      if (Y_out[i] != (unsigned char)(tmpY >> 8))
	abort (); 
      if (X_out[i] != (short)(tmpX >> 16))
	abort (); 
    }

  return 0;
}


/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 2 "vect" { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
