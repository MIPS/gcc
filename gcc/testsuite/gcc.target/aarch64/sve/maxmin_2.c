/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -ffast-math" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (320 / sizeof (TYPE))

#define DEF_MAXMIN(TYPE, NAME, SUFFIX, CMP_OP, CONST)		\
void __attribute__ ((noipa))					\
test_##NAME##_##SUFFIX##_##TYPE (TYPE *restrict r,		\
				 TYPE *restrict a)		\
{								\
  for (int i = 0; i < NUM_ELEMS (TYPE); ++i)			\
    r[i] = a[i] CMP_OP (TYPE) CONST ? a[i] : (TYPE) CONST;	\
}

#define TEST_CONST(T, SUFFIX, CONST)	\
  T (_Float16, max, SUFFIX, >, CONST)	\
  T (float, max, SUFFIX, >, CONST)	\
  T (double, max, SUFFIX, >, CONST)	\
					\
  T (_Float16, min, SUFFIX, <, CONST)	\
  T (float, min, SUFFIX, <, CONST)	\
  T (double, min, SUFFIX, <, CONST)

#define TEST_ALL(T)			\
  TEST_CONST (T, zero, 0.0)		\
  TEST_CONST (T, half, 0.5)		\
  TEST_CONST (T, one, 1.0)		\
  TEST_CONST (T, two, 2.0)

TEST_ALL (DEF_MAXMIN)

/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #0\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #0\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #0\.0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfmaxnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #1\.0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #0\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #0\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #0\.0\n} 1 } } */

/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.h, p[0-7]/m, z[0-9]+\.h, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.s, p[0-7]/m, z[0-9]+\.s, #1\.0\n} 1 } } */
/* { dg-final { scan-assembler-times {\tfminnm\tz[0-9]+\.d, p[0-7]/m, z[0-9]+\.d, #1\.0\n} 1 } } */
