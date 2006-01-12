/* { dg-do compile } */

#include <stdlib.h>
#include <stdarg.h>
#include "tree-vect.h"

/* unify_peeled_chrec creates a new iv in this case during 
   analyze_scalar_evolution. When called from the vectorizer this resulted
   in an ICE since the vectorizer does not expect the code to change during
   analysis.
   */

int
bar (int a[], unsigned n)
{
  int s;
  int i;

  s = 0;
  for (i = 0; i < n; i++)
    s = s + a[i];

  return s;
}

/* { dg-final { cleanup-tree-dump "vect" } } */
