/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve -fno-vect-cost-model" } */

void __attribute__ ((noinline, noclone))
f (double *x, float *y, int n)
{
  for (int i = 0; i < n; ++i)
    {
      x[i * 3 + 18] = x[i * 3 + 0] + y[i];
      x[i * 3 + 19] = x[i * 3 + 1] - y[i];
      x[i * 3 + 20] = x[i * 3 + 2];
    }
}

/* { dg-final { scan-assembler-times {\tld1w\t} 1 } } */
/* { dg-final { scan-assembler-times {\tld3d\t} 2 } } */
/* { dg-final { scan-assembler-times {\tst3d\t} 2 } } */
/* { dg-final { scan-assembler-times {\tpunpklo\t} 1 } } */
/* { dg-final { scan-assembler-times {\tpunpkhi\t} 1 } } */
/* { dg-final { scan-assembler-times {\tuunpklo\t} 1 } } */
/* { dg-final { scan-assembler-times {\tuunpkhi\t} 1 } } */
/* { dg-final { scan-assembler-times {\tfcvt\t} 2 } } */
/* { dg-final { scan-assembler-times {\tfadd\t} 2 } } */
/* { dg-final { scan-assembler-times {\tfsub\t} 2 } } */
