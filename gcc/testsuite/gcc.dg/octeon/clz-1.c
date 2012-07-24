/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tclz\t" } } */

int
clz (unsigned int x)
{
  if (__builtin_clz (x))
    return x;
  return -1;
}
