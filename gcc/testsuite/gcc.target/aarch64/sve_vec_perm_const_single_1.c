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

#define VEC_PERM_SINGLE(TYPE, MASK)			\
TYPE vec_perm_##TYPE (TYPE values1, TYPE values2)	\
{							\
  return __builtin_shuffle (values1, values2, MASK);	\
}

VEC_PERM_SINGLE (v4di,  ((v4di) { 0, 3, 2, 1 }));
VEC_PERM_SINGLE (v8si,  ((v8si) { 3, 7, 1, 0, 2, 4, 4, 2 }));
VEC_PERM_SINGLE (v16hi, ((v16hi) { 8, 7, 5, 4, 11, 12, 13, 0,
				   1, 1, 8, 9, 3, 14, 15, 1 }));
VEC_PERM_SINGLE (v32qi, ((v32qi) { 13, 21, 11, 2, 8, 28, 3, 4,
				   14, 11, 30, 1, 0, 31, 2, 3,
				   4, 5, 11, 23, 24, 11, 12, 9,
				   2, 7, 22, 11, 6, 16, 18, 21 }));
VEC_PERM_SINGLE (v4df,  ((v4di) { 3, 3, 1, 1 }));
VEC_PERM_SINGLE (v8sf,  ((v8si) { 4, 5, 6, 0, 2, 7, 4, 2 }));
VEC_PERM_SINGLE (v16hf, ((v16hi) { 8, 7, 5, 4, 11, 12, 13, 0,
				   1, 1, 8, 9, 3, 14, 15, 1 }));

/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 2 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 1 } } */
