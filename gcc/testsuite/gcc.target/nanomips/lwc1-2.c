/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-m32 -mhard-float -march=32r6" } */

float
foo (float a[])
{
  float test = a[1023] + a[1024];
  return test;
}
/* { dg-final { scan-assembler "\tlwc1\t\\\$f1,4092\\\(\\\$a0\\\)\n" } } */
/* { dg-final { scan-assembler "\tlwc1\t\\\$f0,0\\\(\\\$a3\\\)\n" } } */
