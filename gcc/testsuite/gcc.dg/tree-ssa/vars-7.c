/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized-vars -fdump-rtl-expand" } */

static inline int foo(int i, int k)
{
  int j = i*k;
  return j;
}
int foobar(int l, int n)
{
  int m = foo(l, n);
  return m;
}

/* We want to retain both names for the parameters, l and i and k and n.  */

/* { dg-final { scan-tree-dump "n \\\* l E{ m }" "optimized" } } */
/* { dg-final { scan-tree-dump ": i l" "optimized" } } */
/* { dg-final { scan-tree-dump ": k n" "optimized" } } */
/* { dg-final { scan-rtl-dump "{ i, l }" "expand" } } */
/* { dg-final { scan-rtl-dump "{ k, n }" "expand" } } */
/* { dg-final { scan-rtl-dump "\\\[ m \\\]" "expand" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
/* { dg-final { cleanup-rtl-dump "expand" } } */
