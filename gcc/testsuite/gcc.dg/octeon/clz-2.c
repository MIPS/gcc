/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "dclz\t" } } */

int
clz (unsigned long long x)
{
  if (__builtin_clzll (x))
    return x;
  return -1;
}
