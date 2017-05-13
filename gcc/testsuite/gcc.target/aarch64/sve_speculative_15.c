/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with two consecutive loads from a single global buffer
   which can be aligned without any peeling, and an access stride of 2.  */

#define MAX_ARRAY_SIZE 500

/* Minimum STRIDE_LEVEL is 2.  */
#ifndef STRIDE_LEVEL
#define STRIDE_LEVEL 2
#endif

#define SPEC_LOOP(DATATYPE, ARGTYPE)\
DATATYPE a##DATATYPE[MAX_ARRAY_SIZE];\
ARGTYPE spec_loop_##DATATYPE##_##ARGTYPE (DATATYPE n)\
{\
  ARGTYPE i = -1;\
  do\
    i += 1;\
  while (a##DATATYPE[i*STRIDE_LEVEL] + a##DATATYPE[(i*STRIDE_LEVEL) + 1] < n);\
  return i;\
}

/* TODO: Cannot yet vectorize due to gather load.  */
SPEC_LOOP (int8_t, int8_t)
SPEC_LOOP (int16_t, int16_t)

SPEC_LOOP (int32_t, int32_t)
SPEC_LOOP (int64_t, int64_t)
SPEC_LOOP (float, int32_t)
SPEC_LOOP (double, int64_t)

/* { dg-final { scan-tree-dump-not "loop versioned for vectorization to enhance alignment" "vect" } } */
/* { dg-final { scan-tree-dump-not "Alignment of access forced using peeling" "vect" } } */

/* { dg-final { scan-tree-dump "force alignment of aint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "force alignment of aint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "force alignment of aint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "force alignment of aint64_t" "vect" } } */
/* { dg-final { scan-tree-dump "force alignment of afloat" "vect" } } */
/* { dg-final { scan-tree-dump "force alignment of adouble" "vect" } } */

/* { dg-final { scan-tree-dump "misalign = 1 bytes of ref aint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 2 bytes of ref aint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 4 bytes of ref aint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint64_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 8 bytes of ref aint64_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref afloat" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 4 bytes of ref afloat" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref adouble" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 8 bytes of ref adouble" "vect" } } */

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 4 "vect" } } */
