/* An executable version of branch-2.c.  */
/* { dg-do run } */

#include "branch-helper.h"

void __attribute__((noinline))
NOCOMPRESSION foo (volatile int *x)
{
  if (__builtin_expect (*x == 0, 1))
    OCCUPY_0x1fff8;
}

int
NOCOMPRESSION main (void)
{
  int x = 0;
  int y = 1;

  foo (&x);
  foo (&y);

  return 0;
}
