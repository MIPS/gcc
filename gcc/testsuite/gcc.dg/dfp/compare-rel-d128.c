/* { dg-do run } */
/* { dg-options "-std=gnu99" } */

/* Basic test of runtime comparisons using simple values that are not
   affected by rounding.  */

#define WIDTH 128
#include "compare.h"

int main ()
{
  test_compares ();
  return 0;
}
