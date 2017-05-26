/* { dg-options "-mxlp -fpeephole2 isa_rev>=6 -mnanomips" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (int *b, int *c)
{
  *b += 0x7fffffff;
  *c += 0xffffffff;
}

/* { dg-final { scan-assembler "\taddiu\t\\\$\[0-9\]+,\\\$\[0-9\]+,\[0-9\]+\[ #\]+ADDIU48" } } */
/* { dg-final { scan-assembler "\taddiu\t\\\$\[0-9\]+,\\\$\[0-9\]+,-1" } } */
