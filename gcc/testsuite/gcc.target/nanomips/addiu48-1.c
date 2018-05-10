/* { dg-options "-march=32r6 -fpeephole2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int
foo (int b)
{
  b += 0x12345;
  return b;
}

/* { dg-final { scan-assembler "\taddiu\t\\\$\[ast0-9\]+,\\\$\[ast0-9\]+,\[0-9\]+\[ #\]+ADDIU48" } } */

