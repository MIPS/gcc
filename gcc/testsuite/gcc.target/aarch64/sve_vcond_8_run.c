/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include "sve_vcond_8.c"

#define N 133

#undef TEST
#define TEST(TYPE)							\
  {									\
    int a[N];								\
    TYPE b[N];								\
    for (int i = 0; i < N; ++i)						\
      {									\
	a[i] = i % 5;							\
	b[i] = i % 7;							\
      }									\
    test_##TYPE (a, b, 10, 11, 12, 13, N);				\
    for (int i = 0; i < N; ++i)						\
      if (a[i] != 10 + (i & 1) * 2 + (i % 5 == 0 || i % 7 == 3))	\
	__builtin_abort ();						\
  }

int
main (void)
{
  TEST_ALL
  return 0;
}
