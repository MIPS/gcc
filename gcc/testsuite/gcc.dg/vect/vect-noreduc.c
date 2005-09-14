/* { dg-require-effective-target vect_int } */
/* { dg-do compile } */

#include <stdarg.h>
#include "tree-vect.h"

/* From SPECs gcc benachmark, flow.c:
   The computation looks like a reduction, but it is used
   in the loop, so it cannot be vectorized.  */

void
init_regset_vector (vector, space, nelts, bytes_per_elt)
     int **vector;
     int *space;
     int nelts;
     int bytes_per_elt;
{
  int i;
  int *p = space;

  for (i = 0; i < nelts; i++)
    {
      vector[i] = p;
      p += bytes_per_elt / sizeof (*p);
    }
}

/* { dg-final { scan-tree-dump-times "not vectorized: unsupported pattern" 1 "vect" } } */
/* { dg-final { cleanup-tree-dump "vect" } } */

