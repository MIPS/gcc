/* { dg-do compile } */
/* { dg-options "-mgp64 -march=octeon" } */
/* { dg-final { scan-assembler-times "\tsra\t" 1 } } */
/* { dg-final { scan-assembler-times "\tdsra\t" 1 } } */
/* { dg-final { scan-assembler-not "\tmflo\t" } } */
/* { dg-final { scan-assembler-not "\tmfhi\t" } } */
/* { dg-final { scan-assembler-not "\tmult\t" } } */

/* The divison should be optimized using dmul instead
   of dmult/mult and the shift from the high part is combined
   into one shift.
   This shows up in SPEC. */
int f1(int a)
{
  return a / 13;
}
