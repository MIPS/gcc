/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-forwprop1-details" }  */

double test1 (_Complex double z)
{
  __imag z = 0.;
  return __builtin_cabs (z);
}

/* { dg-final { scan-tree-dump "gimple_simplified to \[^\n\r\]*fabs" "forwprop1" } } */

double test2 (double x)
{
  _Complex z = x;
  __imag z = x;
  return __builtin_cabs (z);
}

/* { dg-final { scan-tree-dump "gimple_simplified to \[^\n\r\]*= _\\d\+ \\* 1.41" "forwprop1" } } */

double test3 (double x)
{
  double y = __builtin_pow (x, 5.);
  return y * x;
}

/* { dg-final { scan-tree-dump "gimple_simplified to \[^\n\r\]*pow \\(x_\\d\+\\(D\\), 6" "forwprop1" } } */
