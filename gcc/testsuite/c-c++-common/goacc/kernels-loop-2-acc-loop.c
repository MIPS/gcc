/* { dg-additional-options "-O2" } */
/* { dg-additional-options "-ftree-parallelize-loops=32" } */
/* { dg-additional-options "-fdump-tree-parloops_oacc_kernels-all" } */
/* { dg-additional-options "-fdump-tree-optimized" } */

/* Check that loops with '#pragma acc loop' tagged gets properly parallelized.  */
#define ACC_LOOP acc loop
#include "kernels-loop-2.c"

/* Check that only three loops are analyzed, and that all can be
   parallelized.  */
/* { dg-final { scan-tree-dump-times "SUCCESS: may be parallelized" 3 "parloops_oacc_kernels" } } */
/* { dg-final { scan-tree-dump-not "FAILED:" "parloops_oacc_kernels" } } */

/* Check that the loop has been split off into a function.  */
/* { dg-final { scan-tree-dump-times "(?n);; Function .*main._omp_fn.0" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-times "(?n);; Function .*main._omp_fn.1" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-times "(?n);; Function .*main._omp_fn.2" 1 "optimized" } } */

/* { dg-final { scan-tree-dump-times "(?n)pragma omp target oacc_parallel.*num_gangs\\(32\\)" 3 "parloops_oacc_kernels" } } */
