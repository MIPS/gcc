/* { dg-options "-mabi=p32 -mxlp -fpeephole2 isa_rev>=7" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

int
foo (void)
{
  int b = 0x12345;
  return b;
}

/* { dg-final { scan-assembler "\tli\t\\\$4,\[0-9\]+" } } */
