/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized-vars" } */

inline int foo(int i, int k)
{
  int j = i*k;
  return j;
}
int foobar(int l)
{
  int m = foo(l, l+1);
  return m;
}

/* We want to retain the name j for i * k in foo and the name
   m for (l + 1) * l in foobar.  Questionably also the name j
   for the latter value, as well as i for l and k for l + 1.  */

/* We manage to do only k for l + 1 and m for (l + 1) * l in foobar.  */

/* { dg-final { scan-tree-dump "k \\\* i E{ j }" "optimized" } } */
/* { dg-final { scan-tree-dump "l \\\+ 1 E{ k }" "optimized" } } */
/* { dg-final { scan-tree-dump "l E{ i l }" "optimized" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "E{ m j }" "optimized" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
