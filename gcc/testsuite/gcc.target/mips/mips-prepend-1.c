/* { dg-options "-mdspr2" } */
/* { dg-final { scan-assembler "prepend\[^\n\]*,10" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

NOMIPS16 int
foo (int x, int y)
{
  return __builtin_mips_prepend (x, y, 42);
}
