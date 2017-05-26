/* { dg-options "-mxlp -fpeephole2 isa_rev>=6 -mnanomips" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int
foo (int b)
{
  b += 0x12345;
  return b;
}

/* { dg-final { scan-assembler "\taddiu\t\\\$\[0-9\]+,\\\$\[0-9\]+,\[0-9\]+\[ #\]+ADDIU48" } } */

