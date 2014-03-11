/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-forwprop2" }  */

double foo (_Complex double z)
{
  __imag z = 0.;
  return __builtin_cabs (z);
}

/* We don't have a lattice in forwprop, so the following needs two steps... */
double bar (double x)
{
  _Complex z = x;
  __imag z = x;
  return __builtin_cabs (z);
}

/* { dg-final { scan-tree-dump-not "cabs" "forwprop2" } } */
/* { dg-final { cleanup-tree-dump "forwprop2" } } */
