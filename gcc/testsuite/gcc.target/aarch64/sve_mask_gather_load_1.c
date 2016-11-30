/* { dg-do compile } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <stdint.h>

#define INVALID_INDEX(TYPE) ((TYPE) 107)
#define IS_VALID_INDEX(TYPE, VAL) (VAL < INVALID_INDEX (TYPE))
#define ODD(VAL) (VAL & 0x1)

/* TODO: This is a bit ugly for floating point types as it involves FP<>INT
   conversions, but I can't find another way of auto-vectorizing the code to
   make use of SVE gather instructions.  */
#define DEF_MASK_GATHER_LOAD(OUTTYPE,LOOKUPTYPE,INDEXTYPE)\
void fun_##OUTTYPE##LOOKUPTYPE##INDEXTYPE (OUTTYPE *__restrict out,\
					   LOOKUPTYPE *__restrict lookup,\
					   INDEXTYPE *__restrict index, int n)\
{\
  int i;\
  for (i = 0; i < n; ++i)\
    {\
      INDEXTYPE x = index[i];\
      if (IS_VALID_INDEX (INDEXTYPE, x))\
        x = lookup[x];\
      out[i] = x;\
    }\
}\

DEF_MASK_GATHER_LOAD (int32_t, int32_t, int32_t)
DEF_MASK_GATHER_LOAD (int64_t, int64_t, int64_t)
DEF_MASK_GATHER_LOAD (uint32_t, uint32_t, uint32_t)
DEF_MASK_GATHER_LOAD (uint64_t, uint64_t, uint64_t)
DEF_MASK_GATHER_LOAD (float, float, int32_t)
DEF_MASK_GATHER_LOAD (double, double, int64_t)

/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d, lsl 3\\\]" 3 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, uxtw 2\\\]" 1 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 2 } } */
