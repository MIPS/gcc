/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve --save-temps" } */

void vcvtf_32 (float *dst, unsigned int *src1, int size)
{
  for (int i = 0; i < size; i++)
    dst[i] = (float) src1[i];
}

void vcvtf_64 (double *dst, unsigned long *src1, int size)
{
  for (int i = 0; i < size; i++)
    dst[i] = (double) src1[i];
}

/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tucvtf\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d\n} 1 } } */
