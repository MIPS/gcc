/* { dg-options "-mabi=32 -mhard-float isa_rev>=7" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (double a[], double b, double c, double d)
{
  float a1 = b + c;
  float a2 = c - d;

  a[1023] = a1;
  a[1024] = a2;
}
/* { dg-final { scan-assembler "\tsdc1\t\\\$f1,4088\\\(\\\$5\\\)\n" } } */
/* { dg-final { scan-assembler "\tsdc1x\t\\\$f0,\\\$4\\\(\\\$7\\\)\n" } } */
