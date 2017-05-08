/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-std=c++11 -O3 -fno-inline -march=armv8-a+sve" } */

#include "sve_vec_init_1.C"

#include <stdlib.h>

#define TEST_INIT_VECTOR(TYPE, VAL)		\
  TYPE r_##TYPE[NUM_ELEMS (TYPE)];		\
  dup_##TYPE (r_##TYPE, VAL);			\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    if (r_##TYPE[i] != VAL)			\
      abort ();

int main (void)
{
  TEST_INIT_VECTOR (int16_t, 0x3976);
  TEST_INIT_VECTOR (int32_t, 0x31232976);
  TEST_INIT_VECTOR (int64_t, 0x9489363731232976LL);

  TEST_INIT_VECTOR (float, -0x1.fe02p10);
  TEST_INIT_VECTOR (double, 0x1.fe02eeeee1p10);

  return 0;
}
