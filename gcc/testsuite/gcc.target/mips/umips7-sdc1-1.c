/* { dg-options "-mabi=p32 -mhard-float isa_rev>=7" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (double a[], double b, double c, double d)
{
  double a1 = b + c;
  double a2 = c - d;

  a[0] = a1;
  a[1] = a2;
}
/* { dg-final { scan-assembler "\tsdc1\t\\\$f2,0\\\(\\\$4\\\)\n" } } */
/* { dg-final { scan-assembler "\tsdc1\t\\\$f4,8\\\(\\\$4\\\)\n" } } */
