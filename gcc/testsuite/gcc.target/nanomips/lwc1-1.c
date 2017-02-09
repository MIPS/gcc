/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-m32 -mhard-float -march=32r6" } */

float
foo (float a[])
{
  float test = a[0] + a[1];
  return test;
}
/* { dg-final { scan-assembler "\tlwc1\t\\\$f1,0\\\(\\\$a\[0-7\]\\\)\n" } } */
/* { dg-final { scan-assembler "\tlwc1\t\\\$f0,4\\\(\\\$a\[0-7\]\\\)\n" } } */
