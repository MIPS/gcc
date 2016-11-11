/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

void
popcount_32 (unsigned int *restrict dst, unsigned int *restrict src, int size)
{
  for (int i = 0; i < size; ++i)
    dst[i] = __builtin_popcount (src[i]);
}

void
popcount_64 (unsigned int *restrict dst, unsigned long *restrict src,
	     int size)
{
  for (int i = 0; i < size; ++i)
    dst[i] = __builtin_popcountl (src[i]);
}

/* { dg-final { scan-assembler-times {\tcnt\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tcnt\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
