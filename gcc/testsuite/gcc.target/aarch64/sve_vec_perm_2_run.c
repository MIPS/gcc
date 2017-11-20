/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include "sve_vec_perm_2.c"

#define N 153

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N], b[N];						\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	b[i] = i * 2 + i % 5;					\
	asm volatile ("" ::: "memory");				\
      }								\
    vec_reverse_##TYPE (a, b, N);				\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	TYPE expected = (N - i - 1) * 2 + (N - i - 1) % 5;	\
	if (a[i] != expected)					\
	  __builtin_abort ();					\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
