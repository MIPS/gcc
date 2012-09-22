/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "madd\t" } } */

int x;

int
f (int a, int b, int c, int d)
{
  int e = a * b;

  return x = e + c * d;
}
