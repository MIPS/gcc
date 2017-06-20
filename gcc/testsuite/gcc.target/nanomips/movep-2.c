/* Check that we can generate the MOVEP instruction.  */
/* { dg-options "-march=32r6 -fpeephole2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int bar (int, int);

int
foo (int n, int a)
{
  return bar (0, 0);
}

/* { dg-final { scan-assembler "\tmovep\t\\\$a0,\\\$a1,\\\$zero,\\\$zero" } } */
