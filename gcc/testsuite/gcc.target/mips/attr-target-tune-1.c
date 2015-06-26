/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32 -mabi=32" } */

/* { dg-final { scan-assembler "test_tune1.*div.*teq.*test_tune1" } } */
/* { dg-final { scan-assembler "test_tune2.*teq.*div.*test_tune2" } } */

long
test_tune1 (long a, long b, long c)
{
  return a * b / c;
}

__attribute__ ((target("tune=mips32r2")))
long
test_tune2 (long a, long b, long c)
{
  return a * b / c;
}
