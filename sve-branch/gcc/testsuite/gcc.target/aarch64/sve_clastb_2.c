/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve --save-temps -fdump-tree-vect-details" } */

#if !defined(TYPE)
#define TYPE unsigned int
#endif

#define N 254

/* Non-simple condition reduction.  */

TYPE
condition_reduction (TYPE *a, TYPE min_v)
{
  TYPE last = 65;

  for (TYPE i = 0; i < N; i++)
    if (a[i] < min_v)
      last = a[i];

  return last;
}

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 1 "vect" } } */
/* { dg-final { scan-tree-dump-not "condition expression based on integer induction." "vect" } } */
/* { dg-final { scan-tree-dump-times "Optimizing condition reduction with CLASTB." 2 "vect" } } */
/* { dg-final { scan-assembler {\tclastb\tw[0-9]+, p[0-7]+, w[0-9]+, z[0-9]+\.s} } } */
