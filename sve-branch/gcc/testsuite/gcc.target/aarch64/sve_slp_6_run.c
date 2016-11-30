/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math" } */

#include "sve_slp_6.c"

#define N (77 * 3)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N], b[3] = { 40, 22, 75 };				\
    for (unsigned int i = 0; i < N; ++i)			\
      a[i] = i * 2 + i % 5;					\
    vec_slp_##TYPE (a, b, N / 3);				\
    TYPE x0 = 40;						\
    TYPE x1 = 22;						\
    TYPE x2 = 75;						\
    for (unsigned int i = 0; i < N; i += 3)			\
      {								\
	x0 += a[i];						\
	x1 += a[i + 1];						\
	x2 += a[i + 2];						\
	asm volatile ("");					\
      }								\
    if (x0 != b[0] || x1 != b[1] || x2 != b[2])			\
      __builtin_abort ();					\
  }

int
main (void)
{
  TEST_ALL (HARNESS)
}
