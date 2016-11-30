/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (4 * (32 / sizeof (TYPE)))
#define INVALID_INDEX(TYPE) ((TYPE) 107)
#define IS_VALID_INDEX(TYPE, VAL) (VAL < INVALID_INDEX (TYPE))

/* TODO: This is a bit ugly for floating point types as it involves FP<>INT
   conversions, but I can't find another way of auto-vectorizing the code to
   make use of SVE gather instructions.  */
#define DEF_MASK_GATHER_LOAD(OUTTYPE,LOOKUPTYPE,INDEXTYPE)\
void fun_##OUTTYPE##LOOKUPTYPE##INDEXTYPE (OUTTYPE *__restrict out,\
                                           LOOKUPTYPE *__restrict lookup,\
                                           INDEXTYPE *__restrict index, INDEXTYPE n)\
{\
  INDEXTYPE i;\
  for (i = 0; i < n; ++i)\
    {\
      LOOKUPTYPE x = index[i];\
      if (IS_VALID_INDEX (LOOKUPTYPE, x))\
        x = lookup[x];\
      out[i] = x;\
    }\
}\

DEF_MASK_GATHER_LOAD (int32_t, int32_t, int8_t)
DEF_MASK_GATHER_LOAD (int64_t, int64_t, int8_t)
DEF_MASK_GATHER_LOAD (int32_t, int32_t, int16_t)
DEF_MASK_GATHER_LOAD (int64_t, int64_t, int16_t)
DEF_MASK_GATHER_LOAD (int64_t, int64_t, int32_t)
DEF_MASK_GATHER_LOAD (uint32_t, uint32_t, uint8_t)
DEF_MASK_GATHER_LOAD (uint64_t, uint64_t, uint8_t)
DEF_MASK_GATHER_LOAD (uint32_t, uint32_t, uint16_t)
DEF_MASK_GATHER_LOAD (uint64_t, uint64_t, uint16_t)
DEF_MASK_GATHER_LOAD (uint64_t, uint64_t, uint32_t)

/* { dg-final { scan-assembler-times "\tpunpklo\\tp\[0-9\]+\.h, p\[0-9\]+\.b" 30 } } */
/* { dg-final { scan-assembler-times "\tpunpkhi\\tp\[0-9\]+\.h, p\[0-9\]+\.b" 30 } } */

/* { dg-final { scan-assembler-times "\tsunpklo\\tz\[0-9\]+\.h, z\[0-9\]+\.b" 2 } } */
/* { dg-final { scan-assembler-times "\tsunpkhi\\tz\[0-9\]+\.h, z\[0-9\]+\.b" 2 } } */
/* { dg-final { scan-assembler-times "\tsunpklo\\tz\[0-9\]+\.s, z\[0-9\]+\.h" 6 } } */
/* { dg-final { scan-assembler-times "\tsunpkhi\\tz\[0-9\]+\.s, z\[0-9\]+\.h" 6 } } */
/* { dg-final { scan-assembler-times "\tsunpklo\\tz\[0-9\]+\.d, z\[0-9\]+\.s" 7 } } */
/* { dg-final { scan-assembler-times "\tsunpkhi\\tz\[0-9\]+\.d, z\[0-9\]+\.s" 7 } } */

/* { dg-final { scan-assembler-times "\tuunpklo\\tz\[0-9\]+\.h, z\[0-9\]+\.b" 2 } } */
/* { dg-final { scan-assembler-times "\tuunpkhi\\tz\[0-9\]+\.h, z\[0-9\]+\.b" 2 } } */
/* { dg-final { scan-assembler-times "\tuunpklo\\tz\[0-9\]+\.s, z\[0-9\]+\.h" 6 } } */
/* { dg-final { scan-assembler-times "\tuunpkhi\\tz\[0-9\]+\.s, z\[0-9\]+\.h" 6 } } */
/* { dg-final { scan-assembler-times "\tuunpklo\\tz\[0-9\]+\.d, z\[0-9\]+\.s" 7 } } */
/* { dg-final { scan-assembler-times "\tuunpkhi\\tz\[0-9\]+\.d, z\[0-9\]+\.s" 7 } } */

/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, uxtw 2\\\]" 6 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 6 } } */
/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d, lsl 3\\\]" 28 } } */
