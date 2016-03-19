/* { dg-additional-options "-foffload-alias=all -fipa-pta" } */
/* Override the compiler's "avoid offloading" decision.
   { dg-additional-options "-foffload-force" } */

#include <stdlib.h>

#define N 2

int
main (void)
{
  unsigned int *a = (unsigned int *)malloc (N * sizeof (unsigned int));
  unsigned int *b = a;
  unsigned int *c = (unsigned int *)malloc (N * sizeof (unsigned int));

#pragma acc kernels pcopyout (a[0:N], b[0:N], c[0:N])
  {
    a[0] = 0;
    b[0] = 1;
    c[0] = a[0];
  }

  if (a[0] != 1 || b[0] != 1 || c[0] != 1)
    abort ();

  free (a);
  free (c);
}
