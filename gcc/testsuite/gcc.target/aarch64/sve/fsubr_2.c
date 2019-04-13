/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O3 --save-temps" } */

#include <stdint.h>

#define DEF_LOOP(VALUE, TYPE, NAME)					\
void __attribute__ ((noipa))						\
test_##NAME##_##TYPE (TYPE *dst, int count)				\
{									\
  for (int i = 0; i < count; ++i)					\
    dst[i] = dst[i] < (TYPE) 5 ? (TYPE) VALUE - dst[i] : (TYPE) 10;	\
}

#define TEST_TYPE(T, TYPE)	\
  T (0.5, TYPE, 0p5);		\
  T (1, TYPE, 1);		\
  T (2, TYPE, 2);		\
  T (3, TYPE, 3);		\
  T (-1, TYPE, minus1);

#define TEST_ALL(T)		\
  TEST_TYPE (T, _Float16)	\
  TEST_TYPE (T, float)		\
  TEST_TYPE (T, double)

TEST_ALL (DEF_LOOP)

/* { dg-final { scan-assembler-not {\tfsub\tz[0-9]+\.b, z} } } */
/* { dg-final { scan-assembler-not {\tfsub\tz[0-9]+\.s, z} } } */
/* { dg-final { scan-assembler-not {\tfsub\tz[0-9]+\.d, z} } } */

/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 3 } } */
/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 3 } } */
/* { dg-final { scan-assembler-times {\tfsub\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 3 } } */

/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #0\.5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #0\.5\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #0\.5\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfsubr\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #1\.0\n} 1 } } */
