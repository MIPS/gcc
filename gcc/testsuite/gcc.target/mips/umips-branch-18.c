/* An executable version of branch-3.c.  */
/* { dg-options "-mmicromips" } */
/* { dg-do run } */

#include "branch-helper.h"

void
foo (volatile int *x)
{
  if (__builtin_expect (*x == 0, 1))
    OCCUPY_0x10000;
}

int
main (void)
{
  int x = 0;
  int y = 1;

  foo (&x);
  foo (&y);

  return 0;
}
