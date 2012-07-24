/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "madd\t" } } */

int
f (int a, int b, int c, int d)
{
  return a * b + c * d;
}
