/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include "sve_mask_gather_load_2.c"

#include <stdio.h>

extern void abort ();

#define ODD(VAL) (VAL & 0x1)
#define INDEX_VEC_INIT(INDEXTYPE)\
  INDEXTYPE index_##INDEXTYPE[NUM_ELEMS (int8_t)];\

#define VEC_INIT(OUTTYPE,LOOKUPTYPE,INDEXTYPE)\
  LOOKUPTYPE lookup_##LOOKUPTYPE[NUM_ELEMS (OUTTYPE)];\
  OUTTYPE out_##OUTTYPE[NUM_ELEMS (OUTTYPE)];\
  {\
    int i;\
    for (i = 0; i < NUM_ELEMS (OUTTYPE); i++)\
      {\
	lookup_##LOOKUPTYPE [i] = i * 2;\
	index_##INDEXTYPE [i] = ODD (i) ? i : INVALID_INDEX (INDEXTYPE);\
      }\
  }

#define TEST_MASK_GATHER_LOAD(OUTTYPE,LOOKUPTYPE,INDEXTYPE)\
  fun_##OUTTYPE##LOOKUPTYPE##INDEXTYPE\
    (out_##OUTTYPE, lookup_##LOOKUPTYPE, index_##INDEXTYPE,\
     NUM_ELEMS (OUTTYPE));\
  {\
    int i;\
    for (i = 0; i < NUM_ELEMS (OUTTYPE); i++)\
      {\
	if (ODD (i) && out_##OUTTYPE[i] != (i * 2))\
	  break;\
	else if (!ODD (i) && out_##OUTTYPE[i] != INVALID_INDEX (OUTTYPE))\
	  break;\
      }\
    if (i < NUM_ELEMS (OUTTYPE))\
      {\
	fprintf (stderr, "out_" # OUTTYPE "[%d] = %d\n",\
		 i, (int) out_##OUTTYPE[i]);\
	abort ();\
      }\
  }

int main()
{
  INDEX_VEC_INIT (int8_t)
  INDEX_VEC_INIT (int16_t)
  INDEX_VEC_INIT (int32_t)
  INDEX_VEC_INIT (uint8_t)
  INDEX_VEC_INIT (uint16_t)
  INDEX_VEC_INIT (uint32_t)

  {
    VEC_INIT (int32_t, int32_t, int8_t)
    TEST_MASK_GATHER_LOAD (int32_t, int32_t, int8_t)
  }
  {
    VEC_INIT (int64_t, int64_t, int8_t)
    TEST_MASK_GATHER_LOAD (int64_t, int64_t, int8_t)
  }
  {
    VEC_INIT (int32_t, int32_t, int16_t)
    TEST_MASK_GATHER_LOAD (int32_t, int32_t, int16_t)
  }
  {
    VEC_INIT (int64_t, int64_t, int16_t)
    TEST_MASK_GATHER_LOAD (int64_t, int64_t, int16_t)
  }
  {
    VEC_INIT (int64_t, int64_t, int32_t)
    TEST_MASK_GATHER_LOAD (int64_t, int64_t, int32_t)
  }
  {
    VEC_INIT (uint32_t, uint32_t, uint8_t)
    TEST_MASK_GATHER_LOAD (uint32_t, uint32_t, uint8_t)
  }
  {
    VEC_INIT (uint64_t, uint64_t, uint8_t)
    TEST_MASK_GATHER_LOAD (uint64_t, uint64_t, uint8_t)
  }
  {
    VEC_INIT (uint32_t, uint32_t, uint16_t)
    TEST_MASK_GATHER_LOAD (uint32_t, uint32_t, uint16_t)
  }
  {
    VEC_INIT (uint64_t, uint64_t, uint16_t)
    TEST_MASK_GATHER_LOAD (uint64_t, uint64_t, uint16_t)
  }
  {
    VEC_INIT (uint64_t, uint64_t, uint32_t)
    TEST_MASK_GATHER_LOAD (uint64_t, uint64_t, uint32_t)
  }

  return 0;
}
