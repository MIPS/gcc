/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "msub\t" } } */

int
f (int a, int b, int c, int d)
{
  return a * b - c * d;
}
