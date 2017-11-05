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

#define UZP1(TYPE, MASK)				\
TYPE uzp1_##TYPE (TYPE values1, TYPE values2)		\
{							\
  return __builtin_shuffle (values1, values2, MASK);	\
}


UZP1 (v4di,  ((v4di) { 0, 2, 4, 6 }));
UZP1 (v8si,  ((v8si) { 0, 2, 4, 6, 8, 10, 12, 14 }));
UZP1 (v16hi, ((v16hi) { 0, 2, 4, 6, 8, 10, 12, 14,
			16, 18, 20, 22, 24, 26, 28, 30 }));
UZP1 (v32qi, ((v32qi) { 0, 2, 4, 6, 8, 10, 12, 14,
			16, 18, 20, 22, 24, 26, 28, 30,
			32, 34, 36, 38, 40, 42, 44, 46,
			48, 50, 52, 54, 56, 58, 60, 62 }));
UZP1 (v4df,  ((v4di) { 0, 2, 4, 6 }));
UZP1 (v8sf,  ((v8si) { 0, 2, 4, 6, 8, 10, 12, 14 }));
UZP1 (v16hf, ((v16hi) { 0, 2, 4, 6, 8, 10, 12, 14,
			16, 18, 20, 22, 24, 26, 28, 30 }));

/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} } } */

/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp1\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
