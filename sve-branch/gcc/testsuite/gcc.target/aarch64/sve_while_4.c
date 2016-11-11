/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include <stdint.h>

#define VEC_PERM(TYPE)					\
TYPE __attribute__ ((weak))				\
vec_while_##TYPE (TYPE *restrict a, unsigned long n)	\
{							\
  for (unsigned long i = 0; i < n; ++i)			\
    a[i] += 1;						\
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

/* { dg-final { scan-assembler-times {\tuqdec} 2 } } */
/* { dg-final { scan-assembler-times {\tuqdecb\tx[0-9]+} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b, xzr,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.b, x[0-9]+,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h, xzr,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.h, x[0-9]+,} 2 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s, xzr,} 3 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.s, x[0-9]+,} 3 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d, xzr,} 3 } } */
/* { dg-final { scan-assembler-times {\twhilelo\tp[0-7]\.d, x[0-9]+,} 3 } } */
