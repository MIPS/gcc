/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "sll" 1 } } */

int
f (long long a, int b)
{
  int c = a;
  return c + b;
}
