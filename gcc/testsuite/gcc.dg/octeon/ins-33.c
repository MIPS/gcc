/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* Make sure we have a truncate for the b variable. */
/* { dg-final { scan-assembler-times "sll" 1 } } */
/* { dg-final { scan-assembler-not "\tor" } } */
/* { dg-final { scan-assembler-not "\tdins" } } */
/* { dg-final { scan-assembler-not "\tcins" } } */
/* { dg-final { scan-assembler "\tins" } } */

unsigned int f(int a, long long b)
{
  int c = b;
  c&=~0xf0;
  c|= (a&0xf)<<4;
  return c;
}
