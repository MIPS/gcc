/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-forwprop1-details" }  */

double test1 (_Complex double z)
{
  __imag z = 0.;
  return __builtin_cabs (z);
}

/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*fabs" "forwprop1" } } */

double test2 (double x)
{
  _Complex z = x;
  __imag z = x;
  return __builtin_cabs (z);
}

/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= _\\d\+ \\* 1.41" "forwprop1" } } */

double test3 (double x)
{
  double y = __builtin_pow (x, 5.);
  return y * x;
}

/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*pow \\(x_\\d\+\\(D\\), 6" "forwprop1" } } */

double test4 (double w)
{
  double x = w * w;
  double y = __builtin_pow (x, -0.5);
  return y * x;
}

/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= w_\\d\+\\(D\\)" "forwprop1" } } */

/* { dg-final { cleanup-tree-dump "forwprop1" } } */
