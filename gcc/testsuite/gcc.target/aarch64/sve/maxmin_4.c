/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (320 / sizeof (TYPE))

#define DEF_MAXMIN(TYPE, NAME, CMP_OP, CONST)			\
void __attribute__ ((noinline, noclone))			\
fun_##NAME##_##CONST##_##TYPE (TYPE *restrict r,		\
			       TYPE *restrict a)		\
{								\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)			\
    r[i] = a[i] CMP_OP (TYPE) CONST ? a[i] : (TYPE) CONST;	\
}

#define TEST_16BIT(T, CONST)	\
  T (uint16_t, max, >, CONST)	\
  T (uint16_t, min, <, CONST)	\
  T (uint32_t, max, >, CONST)	\
  T (uint32_t, min, <, CONST)	\
  T (uint64_t, max, >, CONST)	\
  T (uint64_t, min, <, CONST)

#define TEST_8BIT(T, CONST)	\
  T (uint8_t, max, >, CONST)	\
  T (uint8_t, min, <, CONST)	\
  TEST_16BIT (T, CONST)

#define TEST_ALL(T)		\
  TEST_8BIT (T, 2)		\
  TEST_8BIT (T, 250)		\
  TEST_16BIT (T, 255)		\
  TEST_16BIT (T, 256)

TEST_ALL (DEF_MAXMIN)

/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.b, z[0-9]+\.b, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.h, z[0-9]+\.h, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.s, z[0-9]+\.s, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.d, z[0-9]+\.d, #2\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.b, z[0-9]+\.b, #250\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.h, z[0-9]+\.h, #250\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.s, z[0-9]+\.s, #250\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.d, z[0-9]+\.d, #250\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.h, z[0-9]+\.h, #255\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.s, z[0-9]+\.s, #255\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumax\tz[0-9]+\.d, z[0-9]+\.d, #255\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.b, z[0-9]+\.b, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.h, z[0-9]+\.h, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.s, z[0-9]+\.s, #2\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.d, z[0-9]+\.d, #2\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.b, z[0-9]+\.b, #250\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.h, z[0-9]+\.h, #250\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.s, z[0-9]+\.s, #250\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.d, z[0-9]+\.d, #250\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.h, z[0-9]+\.h, #255\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.s, z[0-9]+\.s, #255\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumin\tz[0-9]+\.d, z[0-9]+\.d, #255\n} 1 } } */
