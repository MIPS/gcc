/* { dg-additional-options "-O2" } */
/* { dg-additional-options "-fdump-tree-optimized" } */
/* { dg-additional-options "-fdump-tree-alias-all" } */
/* { dg-additional-options "-foffload-alias=none" } */

#include <stdlib.h>

#define N (1024 * 512)
#define COUNTERTYPE unsigned int

static void
foo (unsigned int *a, unsigned int *b, unsigned int *c)
{
  for (COUNTERTYPE i = 0; i < N; i++)
    a[i] = i * 2;

  for (COUNTERTYPE i = 0; i < N; i++)
    b[i] = i * 4;

#pragma acc kernels copyin (a[0:N], b[0:N]) copyout (c[0:N])
  {
    for (COUNTERTYPE ii = 0; ii < N; ii++)
      c[ii] = a[ii] + b[ii];
  }

  for (COUNTERTYPE i = 0; i < N; i++)
    if (c[i] != a[i] + b[i])
      abort ();
}

int
main (void)
{
  unsigned int *a;
  unsigned int *b;
  unsigned int *c;

  a = (unsigned int *)malloc (N * sizeof (unsigned int));
  b = (unsigned int *)malloc (N * sizeof (unsigned int));
  c = (unsigned int *)malloc (N * sizeof (unsigned int));

  foo (a, b, c);

  free (a);
  free (b);
  free (c);

  return 0;
}

/* Check that the loop has been split off into a function.  */
/* { dg-final { scan-tree-dump-times "(?n);; Function .*foo.*\\._omp_fn\\.0" 1 "optimized" } } */

/* { dg-final { scan-tree-dump-times "clique 1 base 1" 3 "alias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 2" 1 "alias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 3" 1 "alias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 4" 1 "alias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 5" 1 "alias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 6" 1 "alias" } } */
/* { dg-final { scan-tree-dump-times "clique 1 base 7" 1 "alias" } } */
/* { dg-final { scan-tree-dump-times "(?n)clique .* base .*" 9 "alias" } } */
