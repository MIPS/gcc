/* { dg-options "-mgp64 (-mips16)" } */
/* { dg-final { scan-assembler "\tdivu\t" } } */
/* { dg-final { scan-assembler "\tmflo\t" } } */
/* { dg-final { scan-assembler-not "\tmfhi\t" } } */
/* { dg-skip-if "Removed from mipsr6" { *-img-* } { "*" } { "" } } */

typedef unsigned int SI __attribute__((mode(SI)));

MIPS16 SI
f (SI x, SI y)
{
  return x / y;
}
