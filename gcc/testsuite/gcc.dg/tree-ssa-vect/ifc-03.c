/* APPLE LOCAL file  */
/* { dg-do compile { target powerpc*-*-* i?86-*-* } } */
/* { dg-options "-c -O2 -ftree-vectorize -fdump-tree-vect-details -maltivec" { target powerpc*-*-* } } */
/* { dg-options "-c -O2 -ftree-vectorize -fdump-tree-vect-details -msse" { target i?86-*-* } } */

#include <stdarg.h>
#include <signal.h>

extern float cond;
typedef float afloat __attribute__ ((__aligned__(16)));


#define N 128

void foo (const afloat * __restrict__ in,
	  afloat * __restrict__ out,
	  float cr, float ci)
{
  unsigned int i;

  for (i = 0; i < N; i++) 
    {
      float zr = in[i];

      if (cond < 4.0f)
	out[i] = (zr*zr - cr);
      else
	out[i] = zr;
    }
}

/* { dg-final { scan-tree-dump-times "Applying if-conversion" 1 "vect" } } */
