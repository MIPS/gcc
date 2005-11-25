/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 64
#define SUM_REAL 61504
#define SUM_IMAG 39552

unsigned char
cx_prod(unsigned char * __restrict__ in,
        unsigned char * __restrict__ coefs,
        int n,
        unsigned short *rp,
        unsigned short *ip)
{
  int     i;
  unsigned short  sum_real = 0;
  unsigned short  sum_imag = 0;
  unsigned short tmpi, tmpr;

  for (i = 0; i < n; i++) {
     tmpr = (unsigned short)((unsigned short)(in[2*i] * coefs[2*i]) -
                             (unsigned short)(in[2*i+1] * coefs[2*i+1]));
     tmpi = (unsigned short)((unsigned short)(in[2*i] * coefs[2*i+1]) +
                             (unsigned short)(in[2*i+1] * coefs[2*i]));
     sum_imag += tmpi;
     sum_real += tmpr;
  }
  *rp = sum_real;
  *ip = sum_imag;
}

int main (void)
{
  int i;
  unsigned char input[N];
  unsigned char coeffs[N];
  unsigned short real, imag;
  
  check_vect ();

  for (i = 0; i < N; i++)
    { 
	input[i] = i;
	coeffs[i] = i * 2;
    } 

  cx_prod (input, coeffs,N/2,&real,&imag);

  if (real != SUM_REAL || imag != SUM_IMAG)
    abort (); 

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  { target powerpc*-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
