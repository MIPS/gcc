/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-fre-details -fdump-tree-optimized" } */

struct X {
  int i;
  union {
    int j;
    int k;
    float f;
  } u;
};

int foo(int j)
{
  struct X a;

  a.u.j = j;
  a.u.f = a.u.f;
  a.u.f = a.u.f;
  a.u.j = a.u.j;
  a.u.f = a.u.f;
  return a.u.k;
}

/* { dg-final { scan-tree-dump "Replaced a.u.f with VIEW_CONVERT_EXPR" "fre" } } */
/* { dg-final { scan-tree-dump "Replaced a.u.j with j" "fre" } } */
/* { dg-final { scan-tree-dump "Replaced a.u.k with j" "fre" } } */
/* { dg-final { scan-tree-dump "return j" "optimized" } } */
/* { dg-final { cleanup-tree-dump "fre" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
