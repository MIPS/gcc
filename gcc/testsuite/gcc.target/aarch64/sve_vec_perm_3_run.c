/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include "sve_vec_perm_3.c"

#define N (43 * 8)

#define HARNESS(TYPE)						\
  {								\
    TYPE a[N], b[N], c[N];					\
    for (unsigned int i = 0; i < N; ++i)			\
      {								\
	b[i] = i * 2 + i % 5;					\
	c[i] = i * 3;						\
	asm volatile ("" ::: "memory");				\
      }								\
    vec_zip_##TYPE (a, b, c, N / 8);				\
    for (unsigned int i = 0; i < N / 2; ++i)			\
      {								\
	TYPE expected1 = i * 3;					\
	TYPE expected2 = i * 2 + i % 5;				\
	if (a[i * 2] != expected1 || a[i * 2 + 1] != expected2)	\
	  __builtin_abort ();					\
      }								\
  }

int __attribute__ ((optimize (1)))
main (void)
{
  TEST_ALL (HARNESS)
}
