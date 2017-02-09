/* { dg-options "-mabi=32 -mhard-float isa_rev>=7" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (double a[], double b, double c, double d)
{
  double a1 = b + c;
  double a2 = c - d;

  a[0] = a1;
  a[1] = a2;
}
/* { dg-final { scan-assembler "\tsdc1\t\\\$f1,0\\\(\\\$4\\\)\n" } } */
/* { dg-final { scan-assembler "\tsdc1\t\\\$f0,8\\\(\\\$4\\\)\n" } } */
