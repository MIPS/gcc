/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-gimple" } */

int *xyzzy;

void f1(void)
{
  #pragma omp atomic
    xyzzy++;
}

/* { dg-final { scan-tree-dump-times "xyzzy, 4" 1 "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
