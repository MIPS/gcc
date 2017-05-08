/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include <stdint.h>

#define VEC_PERM(TYPE)						\
TYPE __attribute__ ((weak))					\
vec_slp_##TYPE (TYPE *restrict a, int n)			\
{								\
  for (int i = 0; i < n; ++i)					\
    {								\
      a[i * 2] += 10;						\
      a[i * 2 + 1] += 17;					\
    }								\
}

#define TEST_ALL(T)				\
  T (int8_t)					\
  T (uint8_t)					\
  T (int16_t)					\
  T (uint16_t)					\
  T (int32_t)					\
  T (uint32_t)					\
  T (int64_t)					\
  T (uint64_t)					\
  T (float)					\
  T (double)

TEST_ALL (VEC_PERM)

/* { dg-final { scan-assembler-times {\tld1rh\tz[0-9]+\.h, } 2 } } */
/* { dg-final { scan-assembler-times {\tld1rw\tz[0-9]+\.s, } 2 } } */
/* { dg-final { scan-assembler-times {\tld1rd\tz[0-9]+\.d, } 5 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, #10\n} 2 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, #17\n} 2 } } */
/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 3 } } */
/* { dg-final { scan-assembler-not {\tzip2\t} } } */
