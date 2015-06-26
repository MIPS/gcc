/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32r2 -mabi=32 -mfp64" } */

/* { dg-final { scan-assembler "\.module\tfp=64" } } */
/* { dg-final { scan-assembler "test_fp64.*mtc1.*mthc1.*test_fp64" } } */
/* { dg-final { scan-assembler "test_fp64.*add.d\t\\\$f0,\\\$f1,\\\$f0.*test_fp64" } } */
/* { dg-final { scan-assembler "test_fpxx.*\.set\tfp=xx.*test_fpxx" } } */
/* { dg-final { scan-assembler "test_fpxx.*mtc1.*mthc1.*test_fpxx" } } */
/* { dg-final { scan-assembler "test_fpxx.*add.d\t\\\$f0,\\\$f2,\\\$f0.*test_fpxx" } } */

double
test_fp64 (int x, double a, double b)
{
  return a + b;
}

__attribute__ ((target("fp=xx")))
double
test_fpxx (int x, double a, double b)
{
  return a + b;
}
