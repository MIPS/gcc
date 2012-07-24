/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* The pop instruction does not depend on the word value to be sign extended. */
/* { dg-final { scan-assembler-not "sll\t" } } */

long long f(long long i)
{
  return __builtin_popcount (i);
}


