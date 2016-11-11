/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve" } */

#include "sve_nlogical_1.c"

#define N 128

#define TEST_VNLOGICAL(TYPE)				\
  {							\
    TYPE dst[N], src[N];				\
    for (int i = 0; i < N; ++i)				\
      dst[i] = i ^ 42;					\
    vnlogical_not_##TYPE (dst, N);			\
    for (int i = 0; i < N; ++i)				\
      if (dst[i] != (TYPE) ~(i ^ 42))			\
	__builtin_abort ();				\
    for (int i = 0; i < N; ++i)				\
      {							\
	dst[i] = i ^ 42;				\
	src[i] = i % 5;					\
      }							\
    vnlogical_bic_##TYPE (dst, src, N);			\
    for (int i = 0; i < N; ++i)				\
      if (dst[i] != (TYPE) ((i ^ 42) & ~(i % 5)))	\
	__builtin_abort ();				\
  }

int
main (void)
{
  TEST_VNLOGICAL (char);
  TEST_VNLOGICAL (short);
  TEST_VNLOGICAL (int);
  TEST_VNLOGICAL (long);
  return 0;
}
