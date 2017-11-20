/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve" } */

#include <stdint.h>

#define VEC_PERM(TYPE)						\
TYPE __attribute__ ((noinline, noclone))			\
vec_reverse_##TYPE (TYPE *restrict a, TYPE *restrict b, int n)	\
{								\
  for (int i = 0; i < n; ++i)					\
    a[i] = b[n - i - 1];					\
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

/* { dg-final { scan-assembler-times {\trev\tz[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
/* { dg-final { scan-assembler-times {\trev\tz[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\trev\tz[0-9]+\.s, z[0-9]+\.s\n} 3 } } */
/* { dg-final { scan-assembler-times {\trev\tz[0-9]+\.d, z[0-9]+\.d\n} 3 } } */
