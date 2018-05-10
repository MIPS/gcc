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
