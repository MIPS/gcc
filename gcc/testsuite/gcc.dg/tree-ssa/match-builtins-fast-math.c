/* { dg-do compile } */
/* { dg-options "-O -ffast-math -fdump-tree-forwprop1-details" }  */

double test4 (double w)
{
  double x = w * w;
  double y = __builtin_pow (x, -0.5);
  return y * x;
}

/* { dg-final { scan-tree-dump "gimple_simplified to _\\d\+ = ABS_EXPR <w_\\d\+\\(D\\)" "forwprop1" } } */
