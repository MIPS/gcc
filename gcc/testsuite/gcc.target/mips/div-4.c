/* { dg-options "-mgp64 (-mips16)" } */
/* { dg-final { scan-assembler "\tddivu\t" } } */
/* { dg-final { scan-assembler-not "\tmflo\t" } } */
/* { dg-final { scan-assembler "\tmfhi\t" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

typedef unsigned int DI __attribute__((mode(DI)));

MIPS16 DI
f (DI x, DI y)
{
  return x % y;
}
