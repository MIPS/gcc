/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 16

int ia[N];

int main1 (int n)
{
  unsigned int i;

  /* Loop bound is int and IV is unsigned int.  */
  for (i = 0; i < n; i++)
    {
      ia[i] = 2;
    }

  /* check results:  */
  for (i = 0; i < n; i++)
    {
      if (ia[i] != 2)
        abort();
    }

  return 0;
}

int main (void)
{
  check_vect ();

  return main1 (N);
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

