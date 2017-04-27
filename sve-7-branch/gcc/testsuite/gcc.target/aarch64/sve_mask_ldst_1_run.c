/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include "sve_mask_ldst_1.c"

#include <stdlib.h>

#define DEF_INIT_VECTOR(TYPE)\
  TYPE a_##TYPE[NUM_ELEMS (TYPE) + 1];\
  TYPE b_##TYPE[NUM_ELEMS (TYPE) + 1];\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )\
  {\
    a_##TYPE[i] = (i & 0x1) ? 0 : 4;\
    b_##TYPE[i] = (i & 0x1) ? 4 : 0;\
  }\
  a_##TYPE[NUM_ELEMS (TYPE)] = 101;\
  b_##TYPE[NUM_ELEMS (TYPE)] = 4;

#define TEST_MASK_LOAD(TYPE)\
  maskload_##TYPE (a_##TYPE, b_##TYPE);\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )\
    if (a_##TYPE[i] != 4)\
      {\
        result++;\
      }\
  if (a_##TYPE[NUM_ELEMS (TYPE)] != 101)\
    abort ();

#define TEST_MASK_STORE(TYPE)\
  maskstore_##TYPE (a_##TYPE, b_##TYPE);\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )\
    if (a_##TYPE[i] != 4)\
      {\
        result++;\
      }\
  if (a_##TYPE[NUM_ELEMS (TYPE)] != 101)\
    abort ();

int __attribute__((optimize(1)))
main (int argc, char **argv)
{
  int result = 0;

  {
    DEF_INIT_VECTOR (int8_t);
    DEF_INIT_VECTOR (int16_t);
    DEF_INIT_VECTOR (int32_t);
    DEF_INIT_VECTOR (int64_t);
    DEF_INIT_VECTOR (uint8_t);
    DEF_INIT_VECTOR (uint16_t);
    DEF_INIT_VECTOR (uint32_t);
    DEF_INIT_VECTOR (uint64_t);

    TEST_MASK_LOAD (int8_t);
    TEST_MASK_LOAD (int16_t);
    TEST_MASK_LOAD (int32_t);
    TEST_MASK_LOAD (int64_t);
    TEST_MASK_LOAD (uint8_t);
    TEST_MASK_LOAD (uint16_t);
    TEST_MASK_LOAD (uint32_t);
    TEST_MASK_LOAD (uint64_t);
  }

  {
    DEF_INIT_VECTOR (int8_t);
    DEF_INIT_VECTOR (int16_t);
    DEF_INIT_VECTOR (int32_t);
    DEF_INIT_VECTOR (int64_t);
    DEF_INIT_VECTOR (uint8_t);
    DEF_INIT_VECTOR (uint16_t);
    DEF_INIT_VECTOR (uint32_t);
    DEF_INIT_VECTOR (uint64_t);

    TEST_MASK_STORE (int8_t);
    TEST_MASK_STORE (int16_t);
    TEST_MASK_STORE (int32_t);
    TEST_MASK_STORE (int64_t);
    TEST_MASK_STORE (uint8_t);
    TEST_MASK_STORE (uint16_t);
    TEST_MASK_STORE (uint32_t);
    TEST_MASK_STORE (uint64_t);
  }

  if (result != 0)
    abort ();

  return 0;
}
