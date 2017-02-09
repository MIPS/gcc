/* Check that we save the correct call-saved GPRs and FPRs.  */
/* { dg-options "-march=32r6 -m32 -mhard-float" } */

void bar (void);

void
foo (int x)
{
  __builtin_unwind_init ();
  __builtin_eh_return (x, bar);
}
/* { dg-final { scan-assembler "\\\$s0-\\\$s7" } } */
/* { dg-final { scan-assembler "\\\$(30|fp)" } } */
/* { dg-final { scan-assembler "\\\$f8" } } */
/* { dg-final { scan-assembler "\\\$f9" } } */
/* { dg-final { scan-assembler "\\\$f10" } } */
/* { dg-final { scan-assembler "\\\$f11" } } */
/* { dg-final { scan-assembler "\\\$f12" } } */
/* { dg-final { scan-assembler "\\\$f13" } } */
/* { dg-final { scan-assembler "\\\$f14" } } */
/* { dg-final { scan-assembler "\\\$f15" } } */
/* { dg-final { scan-assembler-not "\\\$f0" } } */
/* { dg-final { scan-assembler-not "\\\$f2" } } */
/* { dg-final { scan-assembler-not "\\\$f6" } } */
/* { dg-final { scan-assembler-not "\\\$f17" } } */
/* { dg-final { scan-assembler-not "\\\$f23" } } */
/* { dg-final { scan-assembler-not "\\\$f25" } } */
/* { dg-final { scan-assembler-not "\\\$f27" } } */
/* { dg-final { scan-assembler-not "\\\$f29" } } */
/* { dg-final { scan-assembler-not "\\\$f31" } } */
