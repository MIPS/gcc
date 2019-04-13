/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (320 / sizeof (TYPE))

#define DEF_MAXMIN(TYPE, NAME, SUFFIX, CMP_OP, CONST)		\
void __attribute__ ((noinline, noclone))			\
test_##NAME##_##SUFFIX##_##TYPE (TYPE *restrict r,		\
				 TYPE *restrict a)		\
{								\
  for (int i = 0; i < NUM_ELEMS (TYPE); ++i)			\
    r[i] = a[i] CMP_OP (TYPE) CONST ? a[i] : (TYPE) CONST;	\
}

#define TEST_16BIT(T, SUFFIX, CONST)	\
  T (int16_t, max, SUFFIX, >, CONST)	\
  T (int16_t, min, SUFFIX, <, CONST)	\
  T (int32_t, max, SUFFIX, >, CONST)	\
  T (int32_t, min, SUFFIX, <, CONST)	\
  T (int64_t, max, SUFFIX, >, CONST)	\
  T (int64_t, min, SUFFIX, <, CONST)

#define TEST_8BIT(T, SUFFIX, CONST)	\
  T (int8_t, max, SUFFIX, >, CONST)	\
  T (int8_t, min, SUFFIX, <, CONST)	\
  TEST_16BIT (T, SUFFIX, CONST)

#define TEST_ALL(T)			\
  TEST_16BIT (T, m129, -129)		\
  TEST_16BIT (T, m128, -128)		\
  TEST_8BIT (T, m126, -126)		\
  TEST_8BIT (T, 0, 0)			\
  TEST_8BIT (T, 125, 125)		\
  TEST_16BIT (T, 126, 126)		\
  TEST_16BIT (T, 128, 128)

TEST_ALL (DEF_MAXMIN)

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, z[0-9]+\.h, #-128\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, z[0-9]+\.s, #-128\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, z[0-9]+\.d, #-128\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.b, z[0-9]+\.b, #-126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, z[0-9]+\.h, #-126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, z[0-9]+\.s, #-126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, z[0-9]+\.d, #-126\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.b, z[0-9]+\.b, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, z[0-9]+\.h, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, z[0-9]+\.s, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, z[0-9]+\.d, #0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.b, z[0-9]+\.b, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, z[0-9]+\.h, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, z[0-9]+\.s, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, z[0-9]+\.d, #125\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.h, z[0-9]+\.h, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.s, z[0-9]+\.s, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmax\tz[0-9]+\.d, z[0-9]+\.d, #126\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, z[0-9]+\.h, #-128\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, z[0-9]+\.s, #-128\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, z[0-9]+\.d, #-128\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.b, z[0-9]+\.b, #-126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, z[0-9]+\.h, #-126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, z[0-9]+\.s, #-126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, z[0-9]+\.d, #-126\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.b, z[0-9]+\.b, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, z[0-9]+\.h, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, z[0-9]+\.s, #0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, z[0-9]+\.d, #0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.b, z[0-9]+\.b, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, z[0-9]+\.h, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, z[0-9]+\.s, #125\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, z[0-9]+\.d, #125\n} 1 } } */

/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.h, z[0-9]+\.h, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.s, z[0-9]+\.s, #126\n} 1 } } */
/* { dg-final { scan-assembler-times {\tsmin\tz[0-9]+\.d, z[0-9]+\.d, #126\n} 1 } } */
