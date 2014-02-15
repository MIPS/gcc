/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tpop\t" } } */

int
popcount (unsigned int x)
{
  return __builtin_popcount (x);
}
