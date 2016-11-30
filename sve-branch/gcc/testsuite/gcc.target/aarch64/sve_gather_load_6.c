/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

void
foo (double *__restrict y, double *__restrict x1,
     double *__restrict x2, int m)
{
  for (int i = 0; i < 256; ++i)
    y[i * m] = x1[i * m] + x2[i * m];
}

/* { dg-final { scan-assembler-times {\tindex\tz[0-9]+\.d, #0, x[0-9]+} 1 } } */
/* { dg-final { scan-assembler-times {\tadd\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d} 1 } } */
/* { dg-final { scan-assembler-not {\torr\tz[0-9]+} } } */
