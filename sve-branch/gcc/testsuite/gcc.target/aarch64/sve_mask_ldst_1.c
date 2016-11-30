/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (73 + sizeof (TYPE))

#define DEF_MASK_LOAD(TYPE)\
void maskload_##TYPE (TYPE *__restrict__ a, TYPE *__restrict__ b)\
{\
  for (TYPE i = 0; i < NUM_ELEMS(TYPE); i++)\
    a[i] = a[i] < 4 ? b[i] : a[i];\
}\

#define DEF_MASK_STORE(TYPE)\
void maskstore_##TYPE (TYPE *__restrict__ a, TYPE *__restrict__ b)\
{\
  for (TYPE i = 0; i < NUM_ELEMS(TYPE); i++)\
    if (b[i] != 0)\
      a[i] = b[i];\
}\

DEF_MASK_LOAD (int8_t)
DEF_MASK_LOAD (int16_t)
DEF_MASK_LOAD (int32_t)
DEF_MASK_LOAD (int64_t)
DEF_MASK_LOAD (uint8_t)
DEF_MASK_LOAD (uint16_t)
DEF_MASK_LOAD (uint32_t)
DEF_MASK_LOAD (uint64_t)

DEF_MASK_STORE (int8_t)
DEF_MASK_STORE (int16_t)
DEF_MASK_STORE (int32_t)
DEF_MASK_STORE (int64_t)
DEF_MASK_STORE (uint8_t)
DEF_MASK_STORE (uint16_t)
DEF_MASK_STORE (uint32_t)
DEF_MASK_STORE (uint64_t)

/* No scalar memory accesses.  */
/* { dg-final { scan-assembler-not {[wx][0-9]+, \[} } } */

/* No scalar memory accesses.  */
/* { dg-final { scan-assembler-not {\tand\t} } } */

/* { dg-final { scan-assembler-times {\tld1b\tz[0-9]+\.b, p[0-7]/z} 6 } } */
/* { dg-final { scan-assembler-times {\tld1h\tz[0-9]+\.h, p[0-7]/z} 6 } } */
/* { dg-final { scan-assembler-times {\tld1w\tz[0-9]+\.s, p[0-7]/z} 6 } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z} 6 } } */

/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7]\.b, p[0-7]/z, z[0-9]+\.b, #3} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7]\.h, p[0-7]/z, z[0-9]+\.h, #3} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7]\.s, p[0-7]/z, z[0-9]+\.s, #3} 1 } } */
/* { dg-final { scan-assembler-times {\tcmpls\tp[0-7]\.d, p[0-7]/z, z[0-9]+\.d, #3} 1 } } */

/* { dg-final { scan-assembler-times {\tcmple\tp[0-7]\.b, p[0-7]/z, z[0-9]+\.b, #3} 1 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7]\.h, p[0-7]/z, z[0-9]+\.h, #3} 1 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7]\.s, p[0-7]/z, z[0-9]+\.s, #3} 1 } } */
/* { dg-final { scan-assembler-times {\tcmple\tp[0-7]\.d, p[0-7]/z, z[0-9]+\.d, #3} 1 } } */

/* { dg-final { scan-assembler-times {\tst1b\tz[0-9]+\.b, p[0-7]} 4 } } */
/* { dg-final { scan-assembler-times {\tst1h\tz[0-9]+\.h, p[0-7]} 4 } } */
/* { dg-final { scan-assembler-times {\tst1w\tz[0-9]+\.s, p[0-7]} 4 } } */
/* { dg-final { scan-assembler-times {\tst1d\tz[0-9]+\.d, p[0-7]} 4 } } */
