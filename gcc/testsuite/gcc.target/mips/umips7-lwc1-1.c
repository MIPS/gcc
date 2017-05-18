/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mabi=p32 -mhard-float isa_rev>=7" } */

float
foo (float a[])
{
  float test = a[0] + a[1];
  return test;
}
/* { dg-final { scan-assembler "\tlwc1\t\\\$f1,0\\\(\\\$4\\\)\n" } } */
/* { dg-final { scan-assembler "\tlwc1\t\\\$f0,4\\\(\\\$4\\\)\n" } } */
