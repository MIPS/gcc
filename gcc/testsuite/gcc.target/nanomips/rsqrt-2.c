/* { dg-do compile } */
/* { dg-options "-ffast-math -mhard-float" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler "\trsqrt.d\t" } } */
/* { dg-final { scan-assembler "\trsqrt.s\t" } } */

extern double sqrt(double);
extern float sqrtf(float);

double foo(double x)
{
  return sqrt(1.0/x);
}

float bar(float x)
{
  return sqrtf(1.0f/x);
}
