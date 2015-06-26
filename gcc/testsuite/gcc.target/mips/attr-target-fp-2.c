/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32r2 -mabi=32 -mfp32" } */

double
test_fp32 (int x, double a, double b)
{
  return a + b;
}

__attribute__ ((target("fp=64")))
double
test_fp64 (int x, double a, double b)
{ /* { dg-error "unsupported combination" } */
  return a + b;
}
