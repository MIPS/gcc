/* { dg-options "-mno-madd isa_rev>=1 " } */
/* { dg-final { scan-assembler-not "madd\t" } } */

NOMIPS16 int
f (int a, int b, int c, int d)
{
  return a * b + c * d;
}
