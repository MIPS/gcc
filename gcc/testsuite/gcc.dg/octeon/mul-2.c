/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dmul\\t" } } */
/* { dg-final { scan-assembler-not "dmult\\t" } } */
/* { dg-final { scan-assembler-not "mflo\\t" } } */

long long
f (long long a, long long b)
{
  return a * b;
}
