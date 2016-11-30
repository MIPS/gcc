/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math" } */

#include "sve_mask_gather_load_4.c"

extern void abort (void);

#define N 57

#undef TEST_LOOP
#define TEST_LOOP(NAME, TYPE)			\
  {						\
    TYPE *ptrs[N];				\
    TYPE data[N * 2];				\
    long mask[N];				\
    for (int i = 0; i < N * 2; ++i)		\
      data[i] = (i / 2) * 4 + i % 2;		\
    TYPE sum = 0;				\
    for (int i = 0; i < N; ++i)			\
      {						\
	mask[i] = i & 15;			\
	ptrs[i] = &data[i * 3 / 2];		\
	sum += mask[i] ? *ptrs[i] : 1;		\
      }						\
    TYPE res = NAME (ptrs, mask, N);		\
    if (res != sum)				\
      abort ();					\
  }

int __attribute__ ((optimize (1)))
main ()
{
  TEST_LOOP (f_s64, long)
  TEST_LOOP (f_u64, unsigned long)
  TEST_LOOP (f_f64, double)
  return 0;
}
