/* { dg-require-effective-target vect_int } */

#include <stdarg.h>
#include "tree-vect.h"
#include <stdio.h>

int main1 (unsigned int *p) {
  int i;
  for (i = 0; i < 64; ++i)
    p[i] = 2;

  /* check results:  */
  for (i = 0; i < 64; ++i)
    if (p[i] != 2)
      abort ();
}

int main (void)
{
  int i = 0;
  unsigned int x[65];

  check_vect ();

  main1 (x);
  main1 (x+1);

  return 0;
}

/* { dg-final { scan-tree-dump-times "vectorized 1 loops" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */
