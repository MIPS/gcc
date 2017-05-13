/* { dg-do compile } */
/* { dg-options "-O3 -fno-inline -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with a load.  Exit condition in the array.  */

#ifndef EXIT_CONDITION
#define EXIT_CONDITION 1
#endif

#define SPEC_LOOP(ARGTYPE)\
ARGTYPE spec_loop_##ARGTYPE (ARGTYPE * array)\
{\
  ARGTYPE i = 0;\
  ARGTYPE r = EXIT_CONDITION + 1;\
  while (r != EXIT_CONDITION)\
  {\
    r = array[i];\
    i++;\
  }\
  return i;\
}

#define SPEC_FP_LOOP(FPTYPE, ARGTYPE)\
ARGTYPE spec_loop_##ARGTYPE##FPTYPE (FPTYPE * array)\
{\
  ARGTYPE i = 0;\
  ARGTYPE r = EXIT_CONDITION + 1;\
  while (r != EXIT_CONDITION)\
  {\
    r = array[i];\
    i++;\
  }\
  return i;\
}

/* TODO: Cannot yet vectorize due to gather load.  */
SPEC_LOOP (int8_t)
SPEC_LOOP (int16_t)

SPEC_LOOP (int32_t)
SPEC_LOOP (int64_t)
SPEC_FP_LOOP (float, int32_t)
SPEC_FP_LOOP (double, int64_t)

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 4 "vect" } } */
/* { dg-final { scan-assembler-times {\tbrka\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b} 4 } } */
/* { dg-final { scan-assembler-not {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.b} } } */
/* { dg-final { scan-assembler-not {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.h} } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.s} 2 } } */
/* { dg-final { scan-assembler-times {\tlastb\tx[0-9]+, p[0-7], z[0-9]+\.d} 2 } } */
/* { dg-final { scan-assembler-not {\tlastb\ts[0-9]+, p[0-7], z[0-9]+\.s} } } */
/* { dg-final { scan-assembler-not {\tlastb\td[0-9]+, p[0-7], z[0-9]+\.d} } } */
