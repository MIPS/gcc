/* { dg-do compile } */
/* { dg-options "-O3 -fno-inline -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with different sizes and no data references .
   Cannot be vectorized.  */

#define SPEC_FP_LOOP(ARGTYPE,INDUCTYPE,FPTYPE)\
FPTYPE spec_fp_loop_##ARGTYPE##INDUCTYPE (ARGTYPE mask, ARGTYPE limit)\
{\
  INDUCTYPE i = 0;\
  FPTYPE f = 0.0;\
  while ((i & mask) != limit)\
    {\
      f += 1;\
      i += 1;\
    }\
  return f;\
}\

SPEC_FP_LOOP (uint32_t, uint32_t, double)

/* { dg-final { scan-tree-dump-times "not vectorized: ncopies is greater than 1" 1 "vect" } } */
/* { dg-final { scan-assembler-not "brka\tp\[0-9\]*.b, p\[0-9\]*\/z, p\[0-9\]*.b" } } */

