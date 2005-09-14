/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"

#define N 4

/* 300.twolf benchmark (Spec2000)  mt.c:145 */

struct array_struct 
{
  int a[N][N];
  int x;
};

int main1 ()
{
  int i, j;
  struct array_struct ta, *t;

  t = &ta;

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
           t->a[i][j] = -t->a[i][j];
        }
    }

  for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
        {
           if (t->a[i][j] == 48)
             abort();
        }
    }

  return 0;
}

int main (void)
{ 
  check_vect ();

  return main1 ();
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect"  } } */
/* { dg-final { scan-tree-dump-times "dependence distance modulo vf == 0" 1 "vect" } } */
/* { dg-final { scan-tree-dump-times "bad data dependence" 0 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

