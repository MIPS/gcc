/* { dg-do compile } */
/* { dg-options "-std=c++11 -O3 -fno-inline -march=armv8-a+sve -fno-tree-loop-distribute-patterns" } */

#include <stdint.h>

#define NUM_ELEMS(TYPE) (1024 / sizeof (TYPE))

#define DEF_LOAD_BROADCAST(TYPE)\
void set_##TYPE (TYPE *__restrict__ a, TYPE *__restrict__ b)\
{\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)\
      a[i] = *b;\
}\

#define DEF_LOAD_BROADCAST_IMM(TYPE,IMM,SUFFIX)\
void set_##TYPE##SUFFIX (TYPE *__restrict__ a)\
{\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)\
      a[i] = IMM;\
}\

/* --- VALID --- */

DEF_LOAD_BROADCAST (int8_t)
DEF_LOAD_BROADCAST (int16_t)
DEF_LOAD_BROADCAST (int32_t)
DEF_LOAD_BROADCAST (int64_t)

DEF_LOAD_BROADCAST_IMM (int16_t, 129, imm_129)
DEF_LOAD_BROADCAST_IMM (int32_t, 129, imm_129)
DEF_LOAD_BROADCAST_IMM (int64_t, 129, imm_129)

DEF_LOAD_BROADCAST_IMM (int16_t, -130, imm_m130)
DEF_LOAD_BROADCAST_IMM (int32_t, -130, imm_m130)
DEF_LOAD_BROADCAST_IMM (int64_t, -130, imm_m130)

DEF_LOAD_BROADCAST_IMM (int16_t, 0x1234, imm_0x1234)
DEF_LOAD_BROADCAST_IMM (int32_t, 0x1234, imm_0x1234)
DEF_LOAD_BROADCAST_IMM (int64_t, 0x1234, imm_0x1234)

DEF_LOAD_BROADCAST_IMM (int16_t, 0xFEDC, imm_0xFEDC)
DEF_LOAD_BROADCAST_IMM (int32_t, 0xFEDC, imm_0xFEDC)
DEF_LOAD_BROADCAST_IMM (int64_t, 0xFEDC, imm_0xFEDC)

DEF_LOAD_BROADCAST_IMM (int32_t, 0x12345678, imm_0x12345678)
DEF_LOAD_BROADCAST_IMM (int64_t, 0x12345678, imm_0x12345678)

DEF_LOAD_BROADCAST_IMM (int32_t, 0xF2345678, imm_0xF2345678)
DEF_LOAD_BROADCAST_IMM (int64_t, 0xF2345678, imm_0xF2345678)

DEF_LOAD_BROADCAST_IMM (int64_t, int64_t (0xFEBA716B12371765), imm_FEBA716B12371765)

/* { dg-final { scan-assembler-times {\tld1rb\tz[0-9]+\.b, p[0-7]/z, } 1 } } */
/* { dg-final { scan-assembler-times {\tld1rh\tz[0-9]+\.h, p[0-7]/z, } 5 } } */
/* { dg-final { scan-assembler-times {\tld1rw\tz[0-9]+\.s, p[0-7]/z, } 7 } } */
/* { dg-final { scan-assembler-times {\tld1rd\tz[0-9]+\.d, p[0-7]/z, } 8 } } */
