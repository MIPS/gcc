/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details" } */

#include <stdint.h>

/* Speculative loop with two loads from the loop phi, which cannot both be aligned.  */

#define SPEC_LOOP(DATATYPE, ARGTYPE)\
ARGTYPE spec_loop_##DATATYPE##_##ARGTYPE (DATATYPE *a, DATATYPE*b, DATATYPE n)\
{\
  DATATYPE x;\
  while ((x=*a++ + *b++) != n);\
  return x;\
}

SPEC_LOOP (int8_t, int8_t)
SPEC_LOOP (int16_t, int16_t)
SPEC_LOOP (int32_t, int32_t)
SPEC_LOOP (int64_t, int64_t)
SPEC_LOOP (float, int32_t)
SPEC_LOOP (double, int64_t)


/* { dg-final { scan-tree-dump-times "unknown alignment, setting loop as first faulting" 12 "vect" } } */
/* { dg-final { scan-tree-dump-not "loop versioned for vectorization to enhance alignment" "vect" } } */
/* { dg-final { scan-tree-dump-not "Alignment of access forced using peeling" "vect" } } */
/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 6 "vect" } } */
