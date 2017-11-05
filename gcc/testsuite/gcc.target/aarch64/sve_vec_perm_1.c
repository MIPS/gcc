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

#define VEC_PERM(TYPE, MASKTYPE)					\
TYPE __attribute__ ((noinline, noclone)) 				\
vec_perm_##TYPE (TYPE values1, TYPE values2, MASKTYPE mask)		\
{									\
  return __builtin_shuffle (values1, values2, mask);			\
}

VEC_PERM (v4di, v4di);
VEC_PERM (v8si, v8si);
VEC_PERM (v16hi, v16hi);
VEC_PERM (v32qi, v32qi);
VEC_PERM (v4df, v4di);
VEC_PERM (v8sf, v8si);
VEC_PERM (v16hf, v16hi);

/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.d, z[0-9]+\.d, z[0-9]+\.d\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.s, z[0-9]+\.s, z[0-9]+\.s\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.h, z[0-9]+\.h, z[0-9]+\.h\n} 4 } } */
/* { dg-final { scan-assembler-times {\ttbl\tz[0-9]+\.b, z[0-9]+\.b, z[0-9]+\.b\n} 2 } } */
