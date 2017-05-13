/* { dg-do compile } */
/* { dg-options "-O3 -fno-inline -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>
#include <stdbool.h>

/* Speculative loop with no data references.  */

/* FIXME: dup of rhs into predicate register is made of horrible code.  */
#define SPEC_LOOP(ARGTYPE,INDUCTYPE)\
INDUCTYPE spec_loop_##ARGTYPE##INDUCTYPE (ARGTYPE mask, ARGTYPE limit,\
					  bool rhs)\
{\
  INDUCTYPE i = 0;\
  bool lhs = (i & mask) != limit;\
  while (lhs == rhs)\
    {\
      i += 1;\
      lhs = (i & mask) != limit;\
    }\
  return i;\
}\

#define SPEC_FP_LOOP(ARGTYPE,INDUCTYPE,FPTYPE)\
INDUCTYPE spec_fp_loop_##ARGTYPE##INDUCTYPE##FPTYPE (ARGTYPE mask, ARGTYPE limit,\
						     bool rhs)\
{\
  INDUCTYPE i = 0;\
  FPTYPE f = 0.0;\
  bool lhs = (i & mask) != limit;\
  while (lhs == rhs)\
    {\
      f += 1;\
      i += 1;\
      lhs = (i & mask) != limit;\
    }\
  return f;\
}\

SPEC_LOOP (uint8_t, uint8_t)
SPEC_LOOP (uint16_t, uint16_t)
SPEC_LOOP (uint32_t, uint32_t)
SPEC_LOOP (uint64_t, uint64_t)

SPEC_LOOP (int8_t, int8_t)
SPEC_LOOP (int16_t, int16_t)
SPEC_LOOP (int32_t, int32_t)
SPEC_LOOP (int64_t, int64_t)

/* Conversions.  */
SPEC_LOOP (uint16_t, uint8_t)

SPEC_LOOP (uint32_t, uint8_t)
SPEC_LOOP (uint32_t, uint16_t)

SPEC_LOOP (uint64_t, uint8_t)
SPEC_LOOP (uint64_t, uint16_t)
SPEC_LOOP (uint64_t, uint32_t)

SPEC_FP_LOOP (uint32_t, uint32_t, float)
SPEC_FP_LOOP (uint64_t, uint64_t, double)

SPEC_FP_LOOP (uint64_t, uint64_t, float)

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 17 "vect" } } */
/* { dg-final { scan-assembler-times {\tbrka\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b} 17 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.b} 5 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.h} 4 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.s} 3 } } */
/* { dg-final { scan-assembler-times {\tlastb\tx[0-9]+, p[0-7], z[0-9]+\.d} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\ts[0-9]+, p[0-7], z[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\td[0-9]+, p[0-7], z[0-9]+\.d} 1 } } */
