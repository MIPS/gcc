/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "msub\t" } } */

int x;

int
f (int a, int b, int c, int d)
{
  return x = a * b - c * d;
}
