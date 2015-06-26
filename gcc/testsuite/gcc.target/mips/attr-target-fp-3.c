/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32r2 -mabi=32 -mfpxx -mno-odd-spreg" } */

/* { dg-final { scan-assembler "\.module\tfp=xx" } } */
/* { dg-final { scan-assembler "test_fp32.*\.set\tfp=32.*test_fp32" } } */
/* { dg-final { scan-assembler "test_fp32.*mtc1.*mtc1.*test_fp32" } } */
/* { dg-final { scan-assembler "test_fp32.*add.d\t\\\$f0,\\\$f2,\\\$f0.*test_fp32" } } */
/* { dg-final { scan-assembler "test_fpxx.*mtc1.*mthc1.*test_fpxx" } } */
/* { dg-final { scan-assembler "test_fpxx.*add.d\t\\\$f0,\\\$f2,\\\$f0.*test_fpxx" } } */
/* { dg-final { scan-assembler "test_fp64_nooddsp.*\.set\tfp=64.*test_fp64_nooddsp" } } */
/* { dg-final { scan-assembler "test_fp64_nooddsp.*\.set\tnooddspreg.*test_fp64_nooddsp" } } */
/* { dg-final { scan-assembler "test_fp64_nooddsp.*ldc1.*ldc1.*test_fp64_nooddsp" } } */
/* { dg-final { scan-assembler "test_fp64_nooddsp.*add.d\t\\\$f0,\\\$f1,\\\$f0.*test_fp64_nooddsp" } } */

__attribute__ ((target("fp=32")))
double
test_fp32 (int x, double a, double b)
{
  return a + b;
}

double
test_fpxx (int x, double a, double b)
{
  return a + b;
}

__attribute__ ((target("fp=64")))
double
test_fp64_nooddsp (int x, double a, double b)
{
  return a + b;
}
