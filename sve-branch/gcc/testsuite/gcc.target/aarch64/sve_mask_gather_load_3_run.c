/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math" } */

#include "sve_mask_gather_load_3.c"

extern void abort (void);

#define N 57

#undef TEST_LOOP
#define TEST_LOOP(NAME, DATA_TYPE, INDEX_TYPE)			\
  {								\
    INDEX_TYPE indices[N];					\
    DATA_TYPE data[N * 2];					\
    for (int i = 0; i < N * 2; ++i)				\
      data[i] = (i / 2) * 4 + i % 2;				\
    DATA_TYPE sum = 0;						\
    for (int i = 0; i < N; ++i)					\
      {								\
	INDEX_TYPE j = (i * 3 / 2) * sizeof (DATA_TYPE);	\
	j &= (1ULL << (sizeof (INDEX_TYPE) * 8 - 1)) - 1;	\
	if (j & 16)						\
	  sum += data[j / sizeof (DATA_TYPE)];			\
	else							\
	  sum += 1;						\
	indices[i] = j;						\
      }								\
    DATA_TYPE res = NAME ((char *) data, indices, N);		\
    if (res != sum)						\
      abort ();							\
  }

int __attribute__ ((optimize (1)))
main ()
{
  TEST_LOOP (f_s32, int, unsigned int)
  TEST_LOOP (f_u32, unsigned int, unsigned int)
  TEST_LOOP (f_f32, float, unsigned int)

  TEST_LOOP (f_s64_s64, long, long)
  TEST_LOOP (f_s64_u64, long, unsigned long)
  TEST_LOOP (f_u64_s64, unsigned long, long)
  TEST_LOOP (f_u64_u64, unsigned long, unsigned long)
  TEST_LOOP (f_f64_s64, double, long)
  TEST_LOOP (f_f64_u64, double, unsigned long)
  return 0;
}
