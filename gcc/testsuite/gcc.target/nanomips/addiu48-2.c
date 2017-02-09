/* { dg-options "-march=32r6 -fpeephole2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (int *b, int *c)
{
  *b += 0x7fffffff;
  *c += 0xffffffff;
}

/* { dg-final { scan-assembler "\taddiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,\[0-9\]+\[ #\]+ADDIU48" } } */
/* { dg-final { scan-assembler "\taddiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,-1" } } */
