/* { dg-additional-options "-O2 -foffload-alias=pointer" } */
/* { dg-additional-options "-fdump-tree-ealias-all -fdump-tree-optimized" } */

void
foo (int *a)
{
  int *p = a;

#pragma acc kernels pcopyin (a[0:1], p[0:1])
  {
    *a = 0;
    *p = 1;
  }
}

/* { dg-final { scan-tree-dump-times " = 0" 1 "optimized" } } */

/* { dg-final { scan-tree-dump-times "clique 1 base 1" 2 "ealias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 2" 1 "ealias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 3" 1 "ealias" } } */
/* { dg-final { scan-tree-dump-times "(?n)clique .* base .*" 4 "ealias" } } */

