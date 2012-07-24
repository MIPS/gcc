/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "dmul" } } */

long long
f (long long a)
{
  return a * 7;
}
