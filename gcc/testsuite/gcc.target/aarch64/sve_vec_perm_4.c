/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -msve-vector-bits=scalable" } */

#include <stdint.h>

#define VEC_PERM(TYPE)					\
TYPE __attribute__ ((noinline, noclone))		\
vec_uzp_##TYPE (TYPE *restrict a, TYPE *restrict b,	\
		 TYPE *restrict c, long n)		\
{							\
  for (long i = 0; i < n; ++i)				\
    {							\
      a[i * 4] = c[i * 8];				\
      b[i * 4] = c[i * 8 + 1];				\
      a[i * 4 + 1] = c[i * 8 + 2];			\
      b[i * 4 + 1] = c[i * 8 + 3];			\
      a[i * 4 + 2] = c[i * 8 + 4];			\
      b[i * 4 + 2] = c[i * 8 + 5];			\
      a[i * 4 + 3] = c[i * 8 + 6];			\
      b[i * 4 + 3] = c[i * 8 + 7];			\
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

/* We could use a single uzp1 and uzp2 per function by implementing
   SLP load permutation for variable width.  XFAIL until then.  */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 3 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 3 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 3 { xfail *-*-* } } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 3 { xfail *-*-* } } } */
/* Delete these if the tests above start passing instead.  */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 24 } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 24 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 24 } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 24 } } */
