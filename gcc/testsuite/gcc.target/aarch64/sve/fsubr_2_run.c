/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math" } */

#include "fsubr_2.c"

#define N 93

#define TEST_LOOP(VALUE, TYPE, NAME)			\
  {							\
    TYPE a[N], orig[N];					\
    for (int i = 0; i < N; ++i)				\
      {							\
	a[i] = orig[i] = i * 0.25 * (i & 1 ? 1 : -1);	\
	asm volatile ("" ::: "memory");			\
      }							\
    test_##NAME##_##TYPE (a, N);			\
    volatile TYPE five = 5, b = VALUE, ten = 10;	\
    for (int i = 0; i < N; ++i)				\
      if (a[i] != (orig[i] < five ? b - orig[i] : ten))	\
	__builtin_abort ();				\
  }

int main ()
{
  TEST_ALL (TEST_LOOP)
  return 0;
}
