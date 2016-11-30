/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-std=c99 -ftree-vectorize -O2 -fno-inline -march=armv8-a+sve" } */

#include "sve_indexoffset_1.c"

#include <string.h>

#define CALL_INDEX_OFFSET_TEST_1(SIGNED, TYPE, ITERTYPE)\
{\
  SIGNED TYPE out[SIZE + 1];\
  SIGNED TYPE in1[SIZE + 1];\
  SIGNED TYPE in2[SIZE + 1];\
  for (int i = 0; i < SIZE + 1; ++i)\
    {\
      in1[i] = (i * 4) ^ i;\
      in2[i] = (i * 2) ^ i;\
    }\
  out[SIZE] = 42;\
  set_##SIGNED##_##TYPE##_##ITERTYPE (out, in1); \
  if (0 != memcmp (out, in1, SIZE * sizeof (TYPE)))\
    return 1;\
  set_##SIGNED##_##TYPE##_##ITERTYPE##_var (out, in2, SIZE); \
  if (0 != memcmp (out, in2, SIZE * sizeof (TYPE)))\
    return 1;\
  if (out[SIZE] != 42)\
    return 1;\
}

#define CALL_INDEX_OFFSET_TEST(SIGNED, TYPE)\
  CALL_INDEX_OFFSET_TEST_1 (SIGNED, TYPE, char) \
  CALL_INDEX_OFFSET_TEST_1 (SIGNED, TYPE, short) \
  CALL_INDEX_OFFSET_TEST_1 (SIGNED, TYPE, int) \
  CALL_INDEX_OFFSET_TEST_1 (SIGNED, TYPE, long)

int
main (void)
{
  CALL_INDEX_OFFSET_TEST (signed, long)
  CALL_INDEX_OFFSET_TEST (unsigned, long)
  CALL_INDEX_OFFSET_TEST (signed, int)
  CALL_INDEX_OFFSET_TEST (unsigned, int)
  CALL_INDEX_OFFSET_TEST (signed, short)
  CALL_INDEX_OFFSET_TEST (unsigned, short)
  CALL_INDEX_OFFSET_TEST (signed, char)
  CALL_INDEX_OFFSET_TEST (unsigned, char)
  return 0;
}
