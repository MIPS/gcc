/* { dg-do compile } */
/* { dg-options "-O3 -fno-inline -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with a conditional load.  */

#define SPEC_LOOP(ARGTYPE,INDUCTYPE)\
INDUCTYPE spec_loop_##ARGTYPE##INDUCTYPE (ARGTYPE mask, ARGTYPE limit,\
					  ARGTYPE * array, ARGTYPE * cond)\
{\
  uint64_t i = 0;\
  INDUCTYPE r = 0;\
  while ((i & mask) != limit)\
  {\
    if (cond[i])\
      r = array[i];\
    i++;\
  }\
  return r;\
}

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

/* { dg-final { scan-tree-dump-not "LOOP VECTORIZED" "vect" } } */
/* { dg-final { scan-tree-dump "speculative mask loads not supported" "vect" } } */
