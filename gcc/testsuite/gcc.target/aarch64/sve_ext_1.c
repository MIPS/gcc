/* { dg-do compile } */
/* { dg-options "-O -march=armv8-a+sve -msve-vector-bits=256" } */

#include <stdint.h>

typedef int64_t v4di __attribute__((vector_size (32)));
typedef int32_t v8si __attribute__((vector_size (32)));
typedef int16_t v16hi __attribute__((vector_size (32)));
typedef int8_t v32qi __attribute__((vector_size (32)));
typedef double v4df __attribute__((vector_size (32)));
typedef float v8sf __attribute__((vector_size (32)));
typedef _Float16 v16hf __attribute__((vector_size (32)));

#define MASK_2(X) X, X + 1
#define MASK_4(X) MASK_2 (X), MASK_2 (X + 2)
#define MASK_8(X) MASK_4 (X), MASK_4 (X + 4)
#define MASK_16(X) MASK_8 (X), MASK_8 (X + 8)
#define MASK_32(X) MASK_16 (X), MASK_16 (X + 16)

#define INDEX_4 v4di
#define INDEX_8 v8si
#define INDEX_16 v16hi
#define INDEX_32 v32qi

#define DUP_LANE(TYPE, NUNITS, INDEX)					     \
  TYPE dup_##INDEX##_##TYPE (TYPE values1, TYPE values2)		     \
  {									     \
    return __builtin_shuffle (values1, values2,				     \
			      ((INDEX_##NUNITS) { MASK_##NUNITS (INDEX) })); \
  }

#define TEST_ALL(T)				\
  T (v4di, 4, 1)				\
  T (v4di, 4, 2)				\
  T (v4di, 4, 3)				\
  T (v8si, 8, 1)				\
  T (v8si, 8, 5)				\
  T (v8si, 8, 7)				\
  T (v16hi, 16, 1)				\
  T (v16hi, 16, 6)				\
  T (v16hi, 16, 15)				\
  T (v32qi, 32, 1)				\
  T (v32qi, 32, 19)				\
  T (v32qi, 32, 31)				\
  T (v4df, 4, 1)				\
  T (v4df, 4, 2)				\
  T (v4df, 4, 3)				\
  T (v8sf, 8, 1)				\
  T (v8sf, 8, 5)				\
  T (v8sf, 8, 7)				\
  T (v16hf, 16, 1)				\
  T (v16hf, 16, 6)				\
  T (v16hf, 16, 15)				\

TEST_ALL (DUP_LANE)

/* { dg-final { scan-assembler-not {\ttbl\t} } } */

/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #1\n} 1 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #2\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #4\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #8\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #12\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #16\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #19\n} 1 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #20\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #24\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #28\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #30\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #31\n} 1 } } */
