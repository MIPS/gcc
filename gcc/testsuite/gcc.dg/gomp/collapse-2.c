/* { dg-do compile } */
/* { dg-options "-O2 -fopenmp -fdump-tree-ssa" } */

#define N 100

int a[N][N];

void
foo (int m, int n)
{
  int i, j;
#pragma omp parallel
#pragma omp for collapse(2) schedule (runtime)
  for (i = 0; i < m; i++)
    for (j = 0; j < n; j++)
      a[i][j] = 1;
}

/* { dg-final { scan-tree-dump-not "(?n)PHI.*count.*\\(D\\)" "ssa" } } */
