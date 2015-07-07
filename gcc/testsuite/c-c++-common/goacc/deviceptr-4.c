/* { dg-additional-options "-fdump-tree-gimple" } */

void
subr (int *a)
{
#pragma acc data deviceptr (a)
#pragma acc parallel
  a[0] += 1.0;
}

/* { dg-final { scan-tree-dump-times "#pragma omp target oacc_parallel.*map\\(force_present:a \\\[len: 8\\\]\\)" 1 "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
