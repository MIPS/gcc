/* Test if we can successfully mix code with micromips.   */
/* { dg-compile } */
/* { dg-options "-mips32r2 -mno-micromips -mfpxx" } */

double
foo (int x, double a, double b)
{
  return a + b;
}

__attribute__ ((micromips,target("fp=64")))
int
test_umips (int x, int a, int b)
{
  return a + b;
}
