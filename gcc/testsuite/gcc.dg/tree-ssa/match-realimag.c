/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-forwprop-details" }  */

/* __real complex (__real x) = x */
double realimag_1(double x)
{
  _Complex double t1 = x;
  double realimag_1_val = __real t1;
  return realimag_1_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to realimag_1_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */
