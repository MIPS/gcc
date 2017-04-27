/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math" } */

#include "sve_slp_5.c"

#define N (141 * 2)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N], b[2] = { 40, 22 };				\
    for (unsigned int i = 0; i < N; ++i)			\
      a[i] = i * 2 + i % 5;					\
    vec_slp_##TYPE (a, b, N / 2);				\
    TYPE x0 = 40;						\
    TYPE x1 = 22;						\
    for (unsigned int i = 0; i < N; i += 2)			\
      {								\
	x0 += a[i];						\
	x1 += a[i + 1];						\
	asm volatile ("");					\
      }								\
    if (x0 != b[0] || x1 != b[1])				\
      __builtin_abort ();					\
  }

int
main (void)
{
  TEST_ALL (HARNESS)
}
