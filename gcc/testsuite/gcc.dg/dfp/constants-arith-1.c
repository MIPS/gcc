/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* A few simple checks on arithmetic operations. */
#include <stdio.h>
#include <stdlib.h>

int main()
{
  /* Compare like-typed positive constants. */
  if (2.99df + 5.1df != 8.09df)
    abort();

  if (5.77df - 2.22dd != 3.55df)
    abort();

  if (2.0dl * 3.7dd * -2 != -14.8df)
    abort();

  if (.18df / -.2df + 1 != 1.e-1dd)
    abort();

  return 0;
}
