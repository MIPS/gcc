/* PR rtl-optimization/21239 */
/* { dg-do run { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-O2 -msse2" } */
#include <emmintrin.h>
#include "i386-cpuid.h"

extern void abort (void);

void
foo (unsigned int x, double *y, const double *z)
{
  __m128d tmp;
  while (x)
    {
      tmp = _mm_load_sd (z);
      _mm_store_sd (y, tmp);
      --x; ++z; ++y;
    }
}

void __attribute__((noinline))
run_tests (void)
{
  unsigned int i;
  double a[16], b[16];
  for (i = 0; i < 16; ++i)
    {
      a[i] = 1;
      b[i] = 2;
    }
  foo (16, a, b);
  for (i = 0; i < 16; ++i)
    {
      if (a[i] != 2)
	abort ();
    }
}

int
main ()
{
  unsigned long cpu_facilities;
  unsigned int i;
  double a[19], b[19];

  cpu_facilities = i386_cpuid ();

  if ((cpu_facilities & (bit_MMX | bit_SSE | bit_SSE2 | bit_CMOV))
      != (bit_MMX | bit_SSE | bit_SSE2 | bit_CMOV))
    /* If host has no vector support, pass.  */
    return 0;

  run_tests ();
  return 0;
}
