/* Check that we can generate the MOVEP instruction.  */
/* { dg-options "-mxlp -fpeephole2 isa_rev>=7" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int bar (int, int);

int
foo (int n, int a)
{
  return bar (0, 0);
}

/* { dg-final { scan-assembler "\tmovep\t\\\$4,\\\$5,\\\$0,\\\$0" } } */
