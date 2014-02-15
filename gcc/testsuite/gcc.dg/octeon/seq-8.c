/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "seq\t" } } */

int
f (long long a, long long b)
{
  return (a == b) + 1;
}
