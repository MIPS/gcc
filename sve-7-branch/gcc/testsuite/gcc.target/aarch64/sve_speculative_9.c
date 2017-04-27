/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with two loads which cannot both be aligned.  */

#ifndef STRIDE_LEVEL
#define STRIDE_LEVEL 1
#endif

#define SPEC_LOOP(DATATYPE, ARGTYPE)\
ARGTYPE spec_loop_##DATATYPE##_##ARGTYPE (DATATYPE *a, DATATYPE*b, DATATYPE n)\
{\
  ARGTYPE i = -1;\
  do\
    i += 1;\
  while (a[i*STRIDE_LEVEL] + b[i*STRIDE_LEVEL] < n);\
  return i;\
}

/* TODO: Cannot yet vectorize due to gather load.  */
SPEC_LOOP (int8_t, int8_t)
SPEC_LOOP (int16_t, int16_t)

SPEC_LOOP (int32_t, int32_t)
SPEC_LOOP (int64_t, int64_t)
SPEC_LOOP (float, int32_t)
SPEC_LOOP (double, int64_t)


/* { dg-final { scan-tree-dump-times "loop versioned for vectorization to enhance alignment" 4 "vect" } } */
/* { dg-final { scan-tree-dump-not "Alignment of access forced using peeling" "vect" } } */
/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 4 "vect" } } */
