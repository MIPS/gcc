/* { dg-do assemble } */
/* { dg-options "-O -march=armv8-a+sve -msve-vector-bits=1024 --save-temps" } */

#include <stdint.h>

typedef int64_t v16di __attribute__((vector_size (128)));
typedef int32_t v32si __attribute__((vector_size (128)));
typedef int16_t v64hi __attribute__((vector_size (128)));
typedef int8_t v128qi __attribute__((vector_size (128)));
typedef double v16df __attribute__((vector_size (128)));
typedef float v32sf __attribute__((vector_size (128)));
typedef _Float16 v64hf __attribute__((vector_size (128)));

#define EXTRACT(ELT_TYPE, TYPE, INDEX)		\
  ELT_TYPE permute_##TYPE##_##INDEX (void)	\
  {						\
    TYPE values;				\
    asm ("" : "=w" (values));			\
    return values[INDEX];			\
  }

#define TEST_ALL(T)				\
  T (int64_t, v16di, 0)				\
  T (int64_t, v16di, 1)				\
  T (int64_t, v16di, 2)				\
  T (int64_t, v16di, 7)				\
  T (int64_t, v16di, 8)				\
  T (int64_t, v16di, 9)				\
  T (int64_t, v16di, 15)			\
  T (int32_t, v32si, 0)				\
  T (int32_t, v32si, 1)				\
  T (int32_t, v32si, 3)				\
  T (int32_t, v32si, 4)				\
  T (int32_t, v32si, 15)			\
  T (int32_t, v32si, 16)			\
  T (int32_t, v32si, 21)			\
  T (int32_t, v32si, 31)			\
  T (int16_t, v64hi, 0)				\
  T (int16_t, v64hi, 1)				\
  T (int16_t, v64hi, 7)				\
  T (int16_t, v64hi, 8)				\
  T (int16_t, v64hi, 31)			\
  T (int16_t, v64hi, 32)			\
  T (int16_t, v64hi, 47)			\
  T (int16_t, v64hi, 63)			\
  T (int8_t, v128qi, 0)				\
  T (int8_t, v128qi, 1)				\
  T (int8_t, v128qi, 15)			\
  T (int8_t, v128qi, 16)			\
  T (int8_t, v128qi, 63)			\
  T (int8_t, v128qi, 64)			\
  T (int8_t, v128qi, 100)			\
  T (int8_t, v128qi, 127)			\
  T (double, v16df, 0)				\
  T (double, v16df, 1)				\
  T (double, v16df, 2)				\
  T (double, v16df, 7)				\
  T (double, v16df, 8)				\
  T (double, v16df, 9)				\
  T (double, v16df, 15)				\
  T (float, v32sf, 0)				\
  T (float, v32sf, 1)				\
  T (float, v32sf, 3)				\
  T (float, v32sf, 4)				\
  T (float, v32sf, 15)				\
  T (float, v32sf, 16)				\
  T (float, v32sf, 21)				\
  T (float, v32sf, 31)				\
  T (_Float16, v64hf, 0)			\
  T (_Float16, v64hf, 1)			\
  T (_Float16, v64hf, 7)			\
  T (_Float16, v64hf, 8)			\
  T (_Float16, v64hf, 31)			\
  T (_Float16, v64hf, 32)			\
  T (_Float16, v64hf, 47)			\
  T (_Float16, v64hf, 63)

TEST_ALL (EXTRACT)

/* { dg-final { scan-assembler-times {\tumov\tx[0-9]+, v[0-9]+\.d\[0\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tx[0-9]+, v[0-9]+\.d\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-not {\tdup\td[0-9]+, v[0-9]+\.d\[0\]\n} } } */
/* { dg-final { scan-assembler-times {\tdup\td[0-9]+, v[0-9]+\.d\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.d, z[0-9]+\.d\[2\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.d, z[0-9]+\.d\[7\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tx[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\td[0-9]+, p[0-7], z[0-9]+\.d\n} 1 } } */

/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.s\[0\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.s\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.s\[3\]\n} 1 } } */
/* { dg-final { scan-assembler-not {\tdup\ts[0-9]+, v[0-9]+\.s\[0\]\n} } } */
/* { dg-final { scan-assembler-times {\tdup\ts[0-9]+, v[0-9]+\.s\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\ts[0-9]+, v[0-9]+\.s\[3\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.s, z[0-9]+\.s\[4\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.s, z[0-9]+\.s\[15\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\ts[0-9]+, p[0-7], z[0-9]+\.s\n} 1 } } */

/* Also used to move the result of a non-Advanced SIMD extract.  */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.h\[0\]\n} 5 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.h\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.h\[7\]\n} 1 } } */
/* { dg-final { scan-assembler-not {\tdup\th[0-9]+, v[0-9]+\.h\[0\]\n} } } */
/* { dg-final { scan-assembler-times {\tdup\th[0-9]+, v[0-9]+\.h\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\th[0-9]+, v[0-9]+\.h\[7\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.h, z[0-9]+\.h\[8\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.h, z[0-9]+\.h\[31\]\n} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\th[0-9]+, p[0-7], z[0-9]+\.h\n} 1 } } */

/* Also used to move the result of a non-Advanced SIMD extract.  */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.b\[0\]\n} 5 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.b\[1\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tumov\tw[0-9]+, v[0-9]+\.b\[15\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.b, z[0-9]+\.b\[16\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tdup\tz[0-9]+\.b, z[0-9]+\.b\[63\]\n} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.b\n} 1 } } */

/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #64\n} 7 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #72\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #84\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #94\n} 2 } } */
/* { dg-final { scan-assembler-times {\text\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b, #100\n} 1 } } */
