/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mabi=32 -mhard-float isa_rev>=7" } */

double
foo (double a[])
{
  double test = a[1023] + a[1024];
  return test;
}
/* { dg-final { scan-assembler "\tldc1\t\\\$f1,4088\\\(\\\$7\\\)\n" } } */
/* { dg-final { scan-assembler "\tldc1x\t\\\$f0,\\\$4\\\(\\\$\[6-7\]\\\)\n" } } */
