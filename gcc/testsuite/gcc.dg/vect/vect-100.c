/* { dg-require-effective-target vect_int } */

#include <stdlib.h>
#include <stdarg.h>
#include "tree-vect.h"

#define N 9

struct extraction
{
  int a[N];
  int b[N];
};

static int a[N] = {1,2,3,4,5,6,7,8,9};
static int b[N] = {2,3,4,5,6,7,8,9,0};

int main1 () {
  int i;
  struct extraction *p;
  
  p = (struct extraction *) malloc (sizeof (struct extraction));

  /* Not vectorizable: p may alias a and/or b.  */
  for (i = 0; i < N; i++)
    {
      p->a[i] = a[i];
      p->b[i] = b[i];
    }

  /* check results: */
  for (i = 0; i < N; i++)
    {
       if (p->a[i] != a[i] || p->b[i] != b[i])
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

