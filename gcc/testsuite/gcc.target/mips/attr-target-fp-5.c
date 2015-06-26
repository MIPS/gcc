/* { dg-compile } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-options "-mips32r2 -mabi=32 -mfp64" } */

__attribute__ ((target("fp=32")))
double
test_fp32 (int x, double a, double b)
{ /* { dg-error "unsupported combination" } */
  return a + b;
}
