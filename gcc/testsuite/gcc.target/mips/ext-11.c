/* { dg-options "-O2 isa_rev>=2" } */
/* { dg-final { scan-assembler-not "and" } } */
/* { dg-final { scan-assembler "ext\t" } } */

#define shift 3
#define mask (((0xffffff)<<shift))

int f(int a)
{
  return a&mask;
}
