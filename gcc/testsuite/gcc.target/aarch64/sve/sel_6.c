/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -msve-vector-bits=256 --save-temps" } */

#include <stdint.h>

typedef int32_t vnx4si __attribute__((vector_size (32)));
typedef float vnx4sf __attribute__((vector_size (32)));

/* Predicate vector: 1 0 0 0 ... */

#define MASK_8		{0, 9, 10, 11, 4, 13, 14, 15}

#define PERMUTE(type, nunits)					\
type permute_##type (type x, type y)				\
{								\
  return __builtin_shuffle (x, y, (vnx4si) MASK_##nunits);	\
}

PERMUTE(vnx4si, 8)

PERMUTE(vnx4sf, 8)

/* { dg-final { scan-assembler-not {\ttbl\t} } } */

/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+\.s, p[0-9]+, z[0-9]+\.s, z[0-9]+\.s\n} 2 } } */

/* { dg-final { scan-assembler-times {\tptrue\tp[0-9]+\.d, vl4\n} 2 } } */
