/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32r2 -mabi=32 -mfpxx -mno-odd-spreg" } */

/* { dg-final { scan-assembler "\.module\tfp=xx" } } */
/* { dg-final { scan-assembler "\.set\tfp=64" } } */
/* { dg-final { scan-assembler "\.set\toddspreg" } } */
/* { dg-final { scan-assembler "mtc1.*mthc1" } } */
/* { dg-final { scan-assembler "add.d\t\\\$f0,\\\$f1,\\\$f0" } } */

__attribute__ ((target("fp=64,odd-spreg")))
double
test_fp64_oddsp (int x, double a, double b)
{
  return a + b;
}
