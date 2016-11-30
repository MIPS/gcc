/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include "sve_var_stride_1.c"
#include "sve_var_stride_1.h"

int
main (void)
{
  for (int n = 0; n < 10; ++n)
    for (int offset = -33; offset <= 33; ++offset)
      test (n, n, offset);
  return 0;
}
