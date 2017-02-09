/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-m32 -mhard-float -march=32r6" } */

double
foo (double a[])
{
  double test = a[0] + a[1];
  return test;
}
/* { dg-final { scan-assembler "\tldc1\t\\\$f1,0\\\(\\\$a\[0-7\]\\\)\n" } } */
/* { dg-final { scan-assembler "\tldc1\t\\\$f0,8\\\(\\\$a\[0-7\]\\\)\n" } } */
