/* { dg-do compile } */
/* { dg-require-effective-target vect_int } */

#include <stdlib.h>
#include <stdarg.h>
#include "tree-vect.h"

#define N 4

int main1 ()
{
  int i;
  size_t ** p;
  size_t * ia[N];

  /* Unknown evolution.  */
  for (i = 0; i < N; i++)
    {
       ia[i] = (int *) *p[i];
    }

  /* check results: */
  for (i = 0; i < N; i++)
    {
      if (ia[i] != (int *) *p[i])
         abort();
    }
  return 0;
}

int main2 (int n)
{
  int i;
  int **ia;

  /* Unknown evolution. Combination of two access functions.  */
  for (i = 0; i < n; i++)
    {
       ia[i][i] = 0;
    }

  /* check results: */
  for (i = 0; i < n; i++)
    {
      if (ia[i][i] != 0)
         abort();
    }
  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 0 loops" 2 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

