/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dpop|pop" } } */

int dpopcount(unsigned long x)
{
  if (__builtin_popcountl (x))
    return x;
  return -1;
}

