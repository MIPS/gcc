/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define INDEX32 uint32_t
#define INDEX64 uint64_t

#include "sve_scatter_store_3.c"

/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7], \[x[0-9]+, z[0-9]+.s, uxtw\]\n} 3 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7], \[x[0-9]+, z[0-9]+.d\]\n} 3 } } */
