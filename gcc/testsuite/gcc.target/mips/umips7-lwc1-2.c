/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mabi=p32 -mhard-float isa_rev>=7" } */

float
foo (float a[])
{
  float test = a[1023] + a[1024];
  return test;
}
/* { dg-final { scan-assembler "\tlwc1\t\\\$f1,4092\\\(\\\$4\\\)\n" } } */
/* { dg-final { scan-assembler "\tlwc1\t\\\$f0,0\\\(\\\$.\\\)\n" } } */
