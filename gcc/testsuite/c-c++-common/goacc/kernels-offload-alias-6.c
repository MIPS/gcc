/* { dg-additional-options "-O2 -foffload-alias=pointer" } */
/* { dg-additional-options "-fdump-tree-ealias-all -fdump-tree-optimized" } */

typedef __SIZE_TYPE__ size_t;
extern void *acc_copyin (void *, size_t);

void
foo (int *a, size_t n)
{
  int *p = (int *)acc_copyin (&a, n);

#pragma acc kernels deviceptr (p) pcopy(a[0:n])
  {
    a = 0;
    *p = 1;
  }
}

/* { dg-final { scan-tree-dump-times "(?n)\\*.* = 0" 1 "optimized" } } */

/* { dg-final { scan-tree-dump-times "clique 1 base 1" 2 "ealias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 2" 1 "ealias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 3" 1 "ealias" } } */
/* { dg-final { scan-tree-dump-times "(?n)clique .* base .*" 4 "ealias" } } */

