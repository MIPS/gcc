/* { dg-do run } */
/* { dg-options "-std=gnu99 -O0" } */

/* Basic test of runtime comparisons using simple values that are not
   affected by rounding.  */

#define WIDTH 64
#include "compare-eq.h"

int main ()
{
  test_compares ();
  return 0;
}
