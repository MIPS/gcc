/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 256

/* The alignment of A is unknown. Yet we do know that both the
   read access and write access have the same alignment. Peeling
   to align on of the accesses will align the other.  */

int
main1 (int * pa)
{
  int i;

  for (i = 0; i < N; i++)
    {
      pa[i] = pa[i] + 1;
    }

  return 0;
}

int main (void)
{
  int i;
  int a[N];

  check_vect ();

  for (i = 0; i < N; i++)
    a[i] = i;

  main1 (a);

  /* check results:  */
  for (i = 0; i < N; i++)
    {
      if (a[i] != i + 1)
        abort ();
    }

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "Vectorizing an unaligned access" 0 "vect" } } */
/* { dg-final { scan-tree-dump-times "Alignment of access forced using peeling" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
