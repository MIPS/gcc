/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dpop\t" } } */

int
popcount (unsigned long long x)
{
  return __builtin_popcountll (x);
}
