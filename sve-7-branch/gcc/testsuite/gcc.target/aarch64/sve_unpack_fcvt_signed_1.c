/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

void unpack_double_int_plus8 (double *d, signed int *s, int size)
{
  for (int i = 0; i < size; i++)
    d[i] = s[i] + 8;
}

/* { dg-final { scan-assembler-times {\tuunpklo\tz[0-9]+\.d, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tuunpkhi\tz[0-9]+\.d, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tscvtf\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.s\n} 2 } } */
