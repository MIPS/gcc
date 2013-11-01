/* { dg-do compile { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-fopenmp -fdump-tree-optimized -O2 -msse2" } */

/* Test that if there is no *inbranch clauses, that both the masked and
   the unmasked version are created.  */

#pragma omp declare simd
int addit(int a, int b, int c)
{
  return a + b;
}

/* { dg-final { scan-tree-dump "clone.* \\(_ZGVxN4vvv_addit" "optimized" } } */
/* { dg-final { scan-tree-dump "clone.* \\(_ZGVxM4vvv_addit" "optimized" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
