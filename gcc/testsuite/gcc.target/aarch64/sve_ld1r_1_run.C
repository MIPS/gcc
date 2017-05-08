/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-std=c++11 -O3 -fno-inline -march=armv8-a+sve -fno-tree-loop-distribute-patterns" } */

#include "sve_ld1r_1.C"

#include <stdlib.h>
#include <stdio.h>

#define TEST_LOAD_BROADCAST(TYPE,IMM)\
  {\
    TYPE v[NUM_ELEMS (TYPE)];\
    TYPE temp = 0;\
    set_##TYPE (v, IMM);\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++ )\
      temp += v[i];\
    result += temp;\
  }\

#define TEST_LOAD_BROADCAST_IMM(TYPE,IMM,SUFFIX)\
  {\
    TYPE v[NUM_ELEMS (TYPE)];\
    TYPE temp = 0;\
    set_##TYPE##SUFFIX (v);\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++ )\
      temp += v[i];\
    result += temp;\
  }\

int main (int argc, char **argv)
{
  long long int result = 0;

  TEST_LOAD_BROADCAST_IMM (int16_t, 129, imm_129)
  TEST_LOAD_BROADCAST_IMM (int32_t, 129, imm_129)
  TEST_LOAD_BROADCAST_IMM (int64_t, 129, imm_129)

  TEST_LOAD_BROADCAST_IMM (int16_t, -130, imm_m130)
  TEST_LOAD_BROADCAST_IMM (int32_t, -130, imm_m130)
  TEST_LOAD_BROADCAST_IMM (int64_t, -130, imm_m130)

  TEST_LOAD_BROADCAST_IMM (int16_t, 0x1234, imm_0x1234)
  TEST_LOAD_BROADCAST_IMM (int32_t, 0x1234, imm_0x1234)
  TEST_LOAD_BROADCAST_IMM (int64_t, 0x1234, imm_0x1234)

  TEST_LOAD_BROADCAST_IMM (int16_t, int16_t (0xFEDC), imm_0xFEDC)
  TEST_LOAD_BROADCAST_IMM (int32_t, 0xFEDC, imm_0xFEDC)
  TEST_LOAD_BROADCAST_IMM (int64_t, 0xFEDC, imm_0xFEDC)

  TEST_LOAD_BROADCAST_IMM (int32_t, 0x12345678, imm_0x12345678)
  TEST_LOAD_BROADCAST_IMM (int64_t, 0x12345678, imm_0x12345678)

  TEST_LOAD_BROADCAST_IMM (int32_t, 0xF2345678, imm_0xF2345678)
  TEST_LOAD_BROADCAST_IMM (int64_t, 0xF2345678, imm_0xF2345678)

  TEST_LOAD_BROADCAST_IMM (int64_t, int64_t (0xFEBA716B12371765),
                           imm_FEBA716B12371765)

  if (result != int64_t (6717319005707226880))
    {
      fprintf (stderr, "result = %lld\n", result);
      abort ();
    }
  return 0;
}
