/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-m32 -mhard-float -march=32r6" } */

double
foo (double a[])
{
  double test = a[1023] + a[1024];
  return test;
}
/* { dg-final { scan-assembler "\tldc1\t\\\$f1,4088\\\(\\\$a3\\\)\n" } } */
/* { dg-final { scan-assembler "\tldc1\t\\\$f1,4088\\\(\\\$a\[2-3\]\\\)\n" } } */
