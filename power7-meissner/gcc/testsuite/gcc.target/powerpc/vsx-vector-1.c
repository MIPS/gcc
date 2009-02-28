/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-options "-O2 -ftree-vectorize -mcpu=power7 -m64" } */
/* { dg-final { scan-assembler "lxvd2u" } } */
/* { dg-final { scan-assembler "stxvd2u" } } */
/* { dg-final { scan-assembler "xvadddp" } } */
/* { dg-final { scan-assembler "xvsubdp" } } */
/* { dg-final { scan-assembler "xvmuldp" } } */
/* { dg-final { scan-assembler "xvdivdp" } } */
/* { dg-final { scan-assembler "xvmadd[am]dp" } } */
/* { dg-final { scan-assembler "xvmsub[am]dp" } } */

#ifndef SIZE
#define SIZE 1024
#endif

double a[SIZE] __attribute__((__aligned__(32)));
double b[SIZE] __attribute__((__aligned__(32)));
double c[SIZE] __attribute__((__aligned__(32)));
double d[SIZE] __attribute__((__aligned__(32)));
double e[SIZE] __attribute__((__aligned__(32)));

void
vector_add (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    a[i] = b[i] + c[i];
}

void
vector_subtract (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    a[i] = b[i] - c[i];
}

void
vector_multiply (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    a[i] = b[i] * c[i];
}

void
vector_multiply_add (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    a[i] = (b[i] * c[i]) + d[i];
}

void
vector_multiply_subtract (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    a[i] = (b[i] * c[i]) - d[i];
}

void
vector_divide (void)
{
  int i;

  for (i = 0; i < SIZE; i++)
    a[i] = b[i] / c[i];
}
