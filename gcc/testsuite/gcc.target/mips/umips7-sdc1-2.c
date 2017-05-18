/* { dg-options "-mabi=p32 -mhard-float isa_rev>=7 -mxlp" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (double a[], double b, double c, double d)
{
  float a1 = b + c;
  float a2 = c - d;

  a[1023] = a1;
  a[1024] = a2;
}
/* { dg-final { scan-assembler "\tsdc1\t\\\$f2,4088\\\(\\\$7\\\)\n" } } */
/* { dg-final { scan-assembler "\tsdc1\t\\\$f4,0\\\(\\\$4\\\)\n" } } */
