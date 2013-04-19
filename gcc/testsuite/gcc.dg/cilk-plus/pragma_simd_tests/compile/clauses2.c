/* { dg-do compile } */
/* { dg-options "-O3 -std=c99 -fcilkplus -fopenmp -fdump-tree-gimple" } */

volatile int *a, *b;

void foo()
{
  int i, j;

#pragma simd assert linear(i : 4, j) vectorlength(4)
  for (i=0; i < 1000; ++i)
    a[i] = b[j];
}

/* { dg-final { scan-tree-dump-times "simd_linear\\(j : 1\\)" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-times "simd_linear\\(i : 4\\)" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-times "simd_vectorlength\\(4\\)" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-times "simd_assert" 1 "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
