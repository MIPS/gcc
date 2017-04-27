/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -ffast-math" } */

#include "sve_gather_load_3.c"

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
	sum += data[j / sizeof (DATA_TYPE)];			\
	indices[i] = j;						\
      }								\
    DATA_TYPE res = NAME ((char *) data, indices, N);		\
    if (res != sum)						\
      abort ();							\
  }

int __attribute__ ((optimize (1)))
main ()
{
  TEST32 (f_s32, int)
  TEST32 (f_u32, unsigned int)
  TEST32 (f_f32, float)

  TEST64 (f_s64, long)
  TEST64 (f_u64, unsigned long)
  TEST64 (f_f64, double)
  return 0;
}
