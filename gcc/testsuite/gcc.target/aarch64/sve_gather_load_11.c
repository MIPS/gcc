/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve --save-temps" } */

void
f (double *restrict a, double *restrict b, short *c, int *d, int n)
{
  for (int i = 0; i < n; i++)
    a[i] = b[c[i] + d[i]];
}

/* { dg-final { scan-assembler-times {\tld1h\tz[0-9]+.h,} 1 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+.s,} 2 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+.d,} 4 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+.d,} 4 } } */
