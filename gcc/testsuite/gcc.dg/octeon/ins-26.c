/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "dins" 1 } } */
/* { dg-final { scan-assembler-not "ext\\t" } } */

unsigned long long z (unsigned long long);

unsigned long long
f (unsigned long long a)
{
  return z (a & 0xffff000000000000ull);
}
