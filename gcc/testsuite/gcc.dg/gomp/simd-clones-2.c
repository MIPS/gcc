/* { dg-do compile { target i?86-*-* x86_64-*-* } } */
/* { dg-options "-fopenmp -fdump-tree-optimized -O -msse2" } */

#pragma omp declare simd inbranch uniform(c) linear(b:66)   // addit.simdclone.2
#pragma omp declare simd notinbranch aligned(c:32) // addit.simdclone.1
int addit(int a, int b, int *c)
{
  return a + b;
}

#pragma omp declare simd uniform(a) aligned(a:32) linear(k:1) notinbranch
float setArray(float *a, float x, int k)
{
  a[k] = a[k] + x;
  return a[k];
}

/* { dg-final { scan-tree-dump "clone.0 \\(_ZGVxN4ua32vl_setArray" "optimized" } } */
/* { dg-final { scan-tree-dump "clone.1 \\(_ZGVxN4vvva32_addit" "optimized" } } */
/* { dg-final { scan-tree-dump "clone.2 \\(_ZGVxM4vl66u_addit" "optimized" } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */
