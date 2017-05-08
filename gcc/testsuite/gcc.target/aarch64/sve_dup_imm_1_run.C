/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-std=c++11 -O3 -fno-inline -march=armv8-a+sve -fno-tree-loop-distribute-patterns" } */

#include "sve_dup_imm_1.C"

#include <stdlib.h>

#define TEST_SET_IMM(TYPE,IMM,SUFFIX)		\
  {						\
    TYPE v[NUM_ELEMS (TYPE)];			\
    set_##TYPE##SUFFIX (v);			\
    for (int i = 0; i < NUM_ELEMS (TYPE); i++ )	\
      if (v[i] != IMM)				\
        result++;				\
  }

int main (int argc, char **argv)
{
  int result = 0;

  TEST_SET_IMM (int8_t,  0, imm_0)
  TEST_SET_IMM (int16_t, 0, imm_0)
  TEST_SET_IMM (int32_t, 0, imm_0)
  TEST_SET_IMM (int64_t, 0, imm_0)

  TEST_SET_IMM (int8_t,  -1, imm_m1)
  TEST_SET_IMM (int16_t, -1, imm_m1)
  TEST_SET_IMM (int32_t, -1, imm_m1)
  TEST_SET_IMM (int64_t, -1, imm_m1)

  TEST_SET_IMM (int8_t,  1, imm_1)
  TEST_SET_IMM (int16_t, 1, imm_1)
  TEST_SET_IMM (int32_t, 1, imm_1)
  TEST_SET_IMM (int64_t, 1, imm_1)

  TEST_SET_IMM (int8_t,  127, imm_127)
  TEST_SET_IMM (int16_t, 127, imm_127)
  TEST_SET_IMM (int32_t, 127, imm_127)
  TEST_SET_IMM (int64_t, 127, imm_127)

  TEST_SET_IMM (int8_t,  -128, imm_m128)
  TEST_SET_IMM (int16_t, -128, imm_m128)
  TEST_SET_IMM (int32_t, -128, imm_m128)
  TEST_SET_IMM (int64_t, -128, imm_m128)

  TEST_SET_IMM (int16_t, 256, imm_256)
  TEST_SET_IMM (int32_t, 256, imm_256)
  TEST_SET_IMM (int64_t, 256, imm_256)

  TEST_SET_IMM (int16_t, 32512, imm_32512)
  TEST_SET_IMM (int32_t, 32512, imm_32512)
  TEST_SET_IMM (int64_t, 32512, imm_32512)

  TEST_SET_IMM (int16_t, -32768, imm_m32768)
  TEST_SET_IMM (int32_t, -32768, imm_m32768)
  TEST_SET_IMM (int64_t, -32768, imm_m32768)

  TEST_SET_IMM (int16_t, 0x0101, imm_01_pat)
  TEST_SET_IMM (int32_t, 0x01010101, imm_01_pat)
  TEST_SET_IMM (int64_t, 0x0101010101010101LL, imm_01_pat)

  TEST_SET_IMM (int32_t, 0x00010001, imm_0001_pat)
  TEST_SET_IMM (int64_t, 0x0001000100010001LL, imm_0001_pat)

  TEST_SET_IMM (int16_t, int16_t (0xFEFE), imm_FE_pat)
  TEST_SET_IMM (int32_t, int32_t (0xFEFEFEFE), imm_FE_pat)
  TEST_SET_IMM (int64_t, int64_t (0xFEFEFEFEFEFEFEFE), imm_FE_pat)

  TEST_SET_IMM (int32_t, int32_t (0xFE00FE00), imm_FE00_pat)
  TEST_SET_IMM (int64_t, int64_t (0xFE00FE00FE00FE00), imm_FE00_pat)

  if (result != 0)
    abort ();

  return 0;
}
