/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -mno-madd" } */
/* { dg-final { scan-assembler-not "madd\t" } } */

int
f (int a, int b, int c, int d)
{
  return a * b + c * d;
}
