/* { dg-do assemble } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256 --save-temps" } */

#include <stdint.h>

typedef int8_t v32qi __attribute__((vector_size(32)));
typedef int16_t v16hi __attribute__((vector_size(32)));
typedef int32_t v8si __attribute__((vector_size(32)));
typedef int64_t v4di __attribute__((vector_size(32)));

#define DO_OP(TYPE)						\
void vmla_##TYPE (TYPE *x, TYPE y, TYPE z)			\
{								\
  register TYPE dst  asm("z0");					\
  register TYPE src1 asm("z2");					\
  register TYPE src2 asm("z4");					\
  dst = *x;							\
  src1 = y;							\
  src2 = z;							\
  asm volatile ("" :: "w" (dst), "w" (src1), "w" (src2));	\
  dst = dst - (src1 * src2);					\
  asm volatile ("" :: "w" (dst));				\
  *x = dst;							\
}

DO_OP (v32qi)
DO_OP (v16hi)
DO_OP (v8si)
DO_OP (v4di)

/* { dg-final { scan-assembler-times {\tmls\tz0\.b, p[0-7]/m, z2\.b, z4\.b\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmls\tz0\.h, p[0-7]/m, z2\.h, z4\.h\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmls\tz0\.s, p[0-7]/m, z2\.s, z4\.s\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmls\tz0\.d, p[0-7]/m, z2\.d, z4\.d\n} 1 } } */
