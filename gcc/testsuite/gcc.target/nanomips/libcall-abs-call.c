/* { dg-do compile } */
/* { dg-options "-march=32r6 -fno-pic -m32 -msoft-float -mcmodel=auto" } */

double test (double a, double b)
{
  return a + b;
}

/* { dg-final { scan-assembler "balc\t__adddf3" } } */