/* { dg-options "-m32 -mhard-float -march=32r6s" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */

void
foo (double a[], double b, double c, double d)
{
  float a1 = b + c;
  float a2 = c - d;

  a[1023] = a1;
  a[1024] = a2;
}
/* { dg-final { scan-assembler "\tsdc1\t\\\$f0,4088\\\(\\\$a3\\\)\n" } } */
/* { dg-final { scan-assembler "\tsdc1\t\\\$f1,0\\\(\\\$a0\\\)\n" } } */
