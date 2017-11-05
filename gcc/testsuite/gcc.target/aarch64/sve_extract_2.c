/* { dg-do assemble } */
/* { dg-options "-O -march=armv8-a+sve -msve-vector-bits=512 --save-temps" } */

#include <stdint.h>

typedef int64_t v8di __attribute__((vector_size (64)));
typedef int32_t v16si __attribute__((vector_size (64)));
typedef int16_t v32hi __attribute__((vector_size (64)));
typedef int8_t v64qi __attribute__((vector_size (64)));
typedef double v8df __attribute__((vector_size (64)));
typedef float v16sf __attribute__((vector_size (64)));
typedef _Float16 v32hf __attribute__((vector_size (64)));

#define EXTRACT(ELT_TYPE, TYPE, INDEX)		\
  ELT_TYPE permute_##TYPE##_##INDEX (void)	\
  {						\
    TYPE values;				\
    asm ("" : "=w" (values));			\
    return values[INDEX];			\
  }

#define TEST_ALL(T)				\
  T (int64_t, v8di, 0)				\
  T (int64_t, v8di, 1)				\
  T (int64_t, v8di, 2)				\
  T (int64_t, v8di, 7)				\
  T (int32_t, v16si, 0)				\
  T (int32_t, v16si, 1)				\
  T (int32_t, v16si, 3)				\
  T (int32_t, v16si, 4)				\
  T (int32_t, v16si, 15)			\
  T (int16_t, v32hi, 0)				\
  T (int16_t, v32hi, 1)				\
  T (int16_t, v32hi, 7)				\
  T (int16_t, v32hi, 8)				\
  T (int16_t, v32hi, 31)			\
  T (int8_t, v64qi, 0)				\
  T (int8_t, v64qi, 1)				\
  T (int8_t, v64qi, 15)				\
  T (int8_t, v64qi, 16)				\
  T (int8_t, v64qi, 63)				\
  T (double, v8df, 0)				\
  T (double, v8df, 1)				\
  T (double, v8df, 2)				\
  T (double, v8df, 7)				\
  T (float, v16sf, 0)				\
  T (float, v16sf, 1)				\
  T (float, v16sf, 3)				\
  T (float, v16sf, 4)				\
  T (float, v16sf, 15)				\
  T (_Float16, v32hf, 0)			\
  T (_Float16, v32hf, 1)			\
  T (_Float16, v32hf, 7)			\
  T (_Float16, v32hf, 8)			\
  T (_Float16, v32hf, 31)

TEST_ALL (EXTRACT)

/* { dg-final { scan-assembler-times {\tumov\tx[0-9]+, v[0-9]+\.d\[0\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tx[0-9]+, v[0-9]+\.d\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-not {\tdup\td[0-9]+, v[0-9]+\.d\[0\]\n} } } */
/* { dg-final { scan-assembler-times {\tdup\td[0-9]+, v[0-9]+\.d\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.d, z[0-9]+\.d\[2\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tx[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.s\[0\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.s\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.s\[3\]\n} 1 } } */
/* { dg-final { scan-assembler-not {\tdup\ts[0-9]+, v[0-9]+\.s\[0\]\n} } } */
/* { dg-final { scan-assembler-times {\tdup\ts[0-9]+, v[0-9]+\.s\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\ts[0-9]+, v[0-9]+\.s\[3\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.s, z[0-9]+\.s\[4\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */

/* Also used to move the result of a non-Advanced SIMD extract.  */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.h\[0\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.h\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.h\[7\]\n} 1 } } */
/* { dg-final { scan-assembler-not {\tdup\th[0-9]+, v[0-9]+\.h\[0\]\n} } } */
/* { dg-final { scan-assembler-times {\tdup\th[0-9]+, v[0-9]+\.h\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\th[0-9]+, v[0-9]+\.h\[7\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.h, z[0-9]+\.h\[8\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\th[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */

/* Also used to move the result of a non-Advanced SIMD extract.  */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.b\[0\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.b\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.b\[15\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.b, z[0-9]+\.b\[16\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */
