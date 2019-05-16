/* { dg-do assemble { target aarch64_asm_sve_ok } } */
/* { dg-options "-O2 -msve-vector-bits=256 --save-temps" } */

#include <stdint.h>

typedef int64_t vnx2di __attribute__((vector_size (32)));
typedef double vnx2df __attribute__((vector_size (32)));

/* Predicate vector: 1 0 0 0 ... */

#define MASK_4 {0, 5, 6, 7}

#define INDEX_4 vnx2di

#define PERMUTE(type, nunits)					\
type permute_##type (type x, type y)				\
{								\
  return __builtin_shuffle (x, y, (vnx2di) MASK_##nunits);	\
}

PERMUTE(vnx2di, 4)

PERMUTE(vnx2df, 4)

/* { dg-final { scan-assembler-not {\ttbl\t} } } */

/* { dg-final { scan-assembler-times {\tsel\tz[0-9]+\.d, p[0-9]+, z[0-9]+\.d, z[0-9]+\.d\n} 2 } } */

/* { dg-final { scan-assembler-times {\tptrue\tp[0-9]+\.b, vl1\n} 2 } } */
