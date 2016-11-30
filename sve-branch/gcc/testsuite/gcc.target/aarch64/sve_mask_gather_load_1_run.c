/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include "sve_mask_gather_load_1.c"

#include <stdio.h>

extern void abort ();

/* TODO: Support widening forms of gather loads and test them here.  */

#define NUM_ELEMS(TYPE) (32 / sizeof (TYPE))

#define INDEX_VEC_INIT(INDEXTYPE)\
  INDEXTYPE index_##INDEXTYPE[NUM_ELEMS (INDEXTYPE)];\

#define VEC_INIT(OUTTYPE,LOOKUPTYPE,INDEXTYPE)\
  LOOKUPTYPE lookup_##LOOKUPTYPE[NUM_ELEMS (LOOKUPTYPE)];\
  OUTTYPE out_##OUTTYPE[NUM_ELEMS (OUTTYPE)];\
  {\
    int i;\
    for (i = 0; i < NUM_ELEMS (INDEXTYPE); i++)\
      {\
	lookup_##LOOKUPTYPE [i] = i * 2;\
	index_##INDEXTYPE [i] = ODD (i) ? i : INVALID_INDEX (INDEXTYPE);\
      }\
  }

#define TEST_MASK_GATHER_LOAD(OUTTYPE,LOOKUPTYPE,INDEXTYPE)\
  fun_##OUTTYPE##LOOKUPTYPE##INDEXTYPE\
    (out_##OUTTYPE, lookup_##LOOKUPTYPE, index_##INDEXTYPE,\
     NUM_ELEMS (INDEXTYPE));\
  {\
    int i;\
    for (i = 0; i < NUM_ELEMS (OUTTYPE); i++)\
      {\
	if (ODD (i) && out_##OUTTYPE[i] != (i * 2))\
	  break;\
	else if (!ODD (i) && out_##OUTTYPE[i] != INVALID_INDEX (INDEXTYPE))\
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
  INDEX_VEC_INIT (int32_t)
  INDEX_VEC_INIT (int64_t)
  INDEX_VEC_INIT (uint32_t)
  INDEX_VEC_INIT (uint64_t)

  VEC_INIT (int32_t, int32_t, int32_t)
  VEC_INIT (int64_t, int64_t, int64_t)
  VEC_INIT (uint32_t, uint32_t, uint32_t)
  VEC_INIT (uint64_t, uint64_t, uint64_t)
  VEC_INIT (float, float, int32_t)
  VEC_INIT (double, double, int64_t)

  TEST_MASK_GATHER_LOAD (int32_t, int32_t, int32_t)
  TEST_MASK_GATHER_LOAD (int64_t, int64_t, int64_t)
  TEST_MASK_GATHER_LOAD (uint32_t, uint32_t, uint32_t)
  TEST_MASK_GATHER_LOAD (uint64_t, uint64_t, uint64_t)
  TEST_MASK_GATHER_LOAD (float, float, int32_t)
  TEST_MASK_GATHER_LOAD (double, double, int64_t)

  return 0;
}
