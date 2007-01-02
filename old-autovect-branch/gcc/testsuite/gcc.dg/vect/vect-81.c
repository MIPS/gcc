/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include <stdio.h>
#include "tree-vect.h"

#define N 16

int main1 ()
{
  int ia[N] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  int ib[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  unsigned int i;

  /* Not vectorized: dependence on previous iteration.  */
  for (i = 1; i < N; i++)
    {
      ia[i] += ia[i-1];
    }

  /* check results:  */
  for (i = 1; i < N; i++)
    {
      if (ia[i] != ib[i])
        abort();
    }

  return 0;
}

int main (void)
{
  check_vect ();

  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

