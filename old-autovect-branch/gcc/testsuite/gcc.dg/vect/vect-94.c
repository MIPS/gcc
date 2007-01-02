/* { dg-require-effective-target vect_int } */

/* PR tree-optimization/18527 */

#include <stdarg.h>
#include "tree-vect.h"

#define N 16

int ia[N];

int main1 (int n)
{
  int i;

  for (i = 0; i <= n; i++)
    {
      ia[i] = 2;
    }

  /* check results:  */
  for (i = 0; i <= n; i++)
    {
      if (ia[i] != 2)
        abort();
    }

  return 0;
}

int main (void)
{
  check_vect ();

  return main1 (N-1);
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" { xfail *-*-* } } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

