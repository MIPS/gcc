/* { dg-options "-mgp64 (-mips16)" } */
/* { dg-final { scan-assembler "\tddiv\t" } } */
/* { dg-final { scan-assembler "\tmflo\t" } } */
/* { dg-final { scan-assembler-not "\tmfhi\t" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

typedef int DI __attribute__((mode(DI)));

MIPS16 DI
f (DI x, DI y)
{
  return x / y;
}
