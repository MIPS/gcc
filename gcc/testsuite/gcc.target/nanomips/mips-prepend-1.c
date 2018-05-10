/* { dg-options "-mdspr3" } */
/* { dg-final { scan-assembler "prepend\[^\n\]*,10" } } */

int
foo (int x, int y)
{
  return __builtin_mips_prepend (x, y, 42);
}
