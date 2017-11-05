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

#define UZP2(TYPE, MASK)				\
TYPE uzp2_##TYPE (TYPE values1, TYPE values2)		\
{							\
  return __builtin_shuffle (values1, values2, MASK);	\
}

UZP2 (v4di,  ((v4di) { 1, 3, 5, 7 }));
UZP2 (v8si,  ((v8si) { 1, 3, 5, 7, 9, 11, 13, 15 }));
UZP2 (v16hi, ((v16hi) { 1, 3, 5, 7, 9, 11, 13, 15,
			17, 19, 21, 23, 25, 27, 29, 31 }));
UZP2 (v32qi, ((v32qi) { 1, 3, 5, 7, 9, 11, 13, 15,
			17, 19, 21, 23, 25, 27, 29, 31,
			33, 35, 37, 39, 41, 43, 45, 47,
			49, 51, 53, 55, 57, 59, 61, 63 }));
UZP2 (v4df,  ((v4di) { 1, 3, 5, 7 }));
UZP2 (v8sf,  ((v8si) { 1, 3, 5, 7, 9, 11, 13, 15 }));
UZP2 (v16hf, ((v16hi) { 1, 3, 5, 7, 9, 11, 13, 15,
			17, 19, 21, 23, 25, 27, 29, 31 }));

/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} } } */
/* { dg-final { scan-assembler-not {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} } } */

/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\tuzp2\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
