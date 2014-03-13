/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-forwprop" }  */

double test1 (_Complex double z)
{
  __imag z = 0.;
  return __builtin_cabs (z);
}

double test2 (double x)
{
  _Complex z = x;
  __imag z = x;
  return __builtin_cabs (z);
}

double test3 (double x)
{
  double y = __builtin_pow (x, 5.);
  return y * x;
}

/* { dg-final { scan-tree-dump-not "cabs" "forwprop1" } } */
/* { dg-final { scan-tree-dump "pow \\\(\[^,\]*, 6" "forwprop1" } } */
/* { dg-final { cleanup-tree-dump "forwprop2" } } */
