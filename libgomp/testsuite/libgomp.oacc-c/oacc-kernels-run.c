/* { dg-do run } */
/* { dg-options "-ftree-parallelize-loops=32 -O2 -std=c99" } */

#include <stdlib.h>
#include <stdio.h>

#define N (1024 * 512)
#define N_REF 4293394432

#if 1
#define COUNTERTYPE unsigned int
#else
#define COUNTERTYPE int
#endif

int
main (void)
{
  unsigned int i;

  unsigned int *__restrict a;
  unsigned int *__restrict b;
  unsigned int *__restrict c;

  a = malloc (N * sizeof (unsigned int));
  b = malloc (N * sizeof (unsigned int));
  c = malloc (N * sizeof (unsigned int));


  for (COUNTERTYPE i = 0; i < N; i++)
    a[i] = i * 2;

  for (COUNTERTYPE i = 0; i < N; i++)
    b[i] = i * 4;

#pragma acc kernels pcopyin(a[0:N], b[0:N]) pcopyout(c[0:N])
  {
    for (COUNTERTYPE ii = 0; ii < N; ii++)
      c[ii] = a[ii] + b[ii];
  }

  {
    unsigned int sum = 0;

    for (COUNTERTYPE i = 0; i < N; i++)
      sum += c[i];

  printf ("sum: %u\n", sum);

  if (sum != N_REF)
    abort ();
  }

  free (a);
  free (b);
  free (c);

  return 0;
}
