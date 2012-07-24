/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "exts" } } */
/* { dg-final { scan-assembler-not "sll" } } */

int
f (unsigned long long a)
{
  return a >> 18;
}
