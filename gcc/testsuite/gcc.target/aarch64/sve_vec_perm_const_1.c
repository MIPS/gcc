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

#define VEC_PERM_CONST(TYPE, MASK)			\
TYPE __attribute__ ((noinline, noclone)) 		\
vec_perm_##TYPE (TYPE values1, TYPE values2)		\
{							\
  return __builtin_shuffle (values1, values2, MASK);	\
}

VEC_PERM_CONST (v4di,  ((v4di) { 4, 3, 6, 1 }));
VEC_PERM_CONST (v8si,  ((v8si) { 3, 9, 11, 12, 2, 4, 4, 2 }));
VEC_PERM_CONST (v16hi, ((v16hi) { 8, 27, 5, 4, 21, 12, 13, 0,
				  22, 1, 8, 9, 3, 24, 15, 1 }));
VEC_PERM_CONST (v32qi, ((v32qi) { 13, 31, 11, 2, 48, 28, 3, 4,
				  54, 11, 30, 1, 0, 61, 2, 3,
				  4, 5, 11, 63, 24, 11, 42, 39,
				  2, 57, 22, 11, 6, 16, 18, 21 }));
VEC_PERM_CONST (v4df,  ((v4di) { 7, 3, 2, 1 }));
VEC_PERM_CONST (v8sf,  ((v8si) { 1, 9, 13, 11, 2, 5, 4, 2 }));
VEC_PERM_CONST (v16hf, ((v16hi) { 8, 27, 5, 4, 21, 12, 13, 0,
				  22, 1, 8, 9, 3, 24, 15, 1 }));

/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
