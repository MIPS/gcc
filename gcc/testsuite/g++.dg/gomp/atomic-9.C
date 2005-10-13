/* { dg-do compile } */
/* { dg-options "-fopenmp -fdump-tree-gimple" } */

volatile int *bar(void);

void f1(void)
{
  #pragma omp atomic
    *bar() += 1;
}

/* { dg-final { scan-tree-dump-times "__sync_fetch_and_add" 1 "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
