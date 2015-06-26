/* { dg-compile } */

__attribute__ ((target("fp=31")))
double
test_fp64 (int x, double a, double b)
{ /* { dg-error "unrecognized argument in the target attribute fp=" } */
  return a + b;
}
