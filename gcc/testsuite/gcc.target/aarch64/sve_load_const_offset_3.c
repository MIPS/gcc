/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -save-temps" } */

void
f (unsigned int *restrict a, unsigned char *restrict b, int n)
{
  for (int i = 0; i < n; ++i)
    a[i] += b[i];
}

/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, #1, mul vl\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, #2, mul vl\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z, \[x[0-9]+, #3, mul vl\]\n} 1 } } */
