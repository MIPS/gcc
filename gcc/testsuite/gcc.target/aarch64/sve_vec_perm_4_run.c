/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include "sve_vec_perm_4.c"

#define N (43 * 8)

#define HARNESS(TYPE)					\
  {							\
    TYPE a[N], b[N], c[N];				\
    for (unsigned int i = 0; i < N; ++i)		\
      {							\
	c[i] = i * 2 + i % 5;				\
	asm volatile ("" ::: "memory");			\
      }							\
    vec_uzp_##TYPE (a, b, c, N / 8);			\
    for (unsigned int i = 0; i < N; ++i)		\
      {							\
	TYPE expected = i * 2 + i % 5;			\
	if ((i & 1 ? b[i / 2] : a[i / 2]) != expected)	\
	  __builtin_abort ();				\
      }							\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
