/* { dg-do compile } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (128 / sizeof (TYPE))

#define DUP_FN(TYPE)				\
void dup_##TYPE (TYPE *r, TYPE v)		\
{						\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    r[i] = v;					\
}

DUP_FN (int16_t)
DUP_FN (int32_t)
DUP_FN (int64_t)
DUP_FN (float)
DUP_FN (double)

/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.h, w[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.s, w[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, x[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.s, s[0-9]+\n} 1 } } */
/* { dg-final { scan-assembler-times {\tmov\tz[0-9]+\.d, d[0-9]+\n} 1 } } */
