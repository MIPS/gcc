/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -msve-vector-bits=scalable" } */

#include <stdint.h>

#define VEC_PERM(TYPE)					\
TYPE __attribute__ ((noinline, noclone))		\
vec_zip_##TYPE (TYPE *restrict a, TYPE *restrict b,	\
		TYPE *restrict c, long n)		\
{							\
  for (long i = 0; i < n; ++i)				\
    {							\
      a[i * 8] = c[i * 4];				\
      a[i * 8 + 1] = b[i * 4];				\
      a[i * 8 + 2] = c[i * 4 + 1];			\
      a[i * 8 + 3] = b[i * 4 + 1];			\
      a[i * 8 + 4] = c[i * 4 + 2];			\
      a[i * 8 + 5] = b[i * 4 + 2];			\
      a[i * 8 + 6] = c[i * 4 + 3];			\
      a[i * 8 + 7] = b[i * 4 + 3];			\
    }							\
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

/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 24 } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 24 } } */
/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 24 } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 24 } } */
/* Currently we can't use SLP for groups bigger than 128 bits.  */
/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 36 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 36 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tzip1\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 36 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tzip2\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 36 { xfail *-*-* } } } */
