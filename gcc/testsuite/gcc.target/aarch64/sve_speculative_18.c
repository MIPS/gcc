/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details" } */

/* Speculative loop with two loads which cannot both be aligned.
   Will use first faulting (instead of versioning) to cope with alignment
   issues.  */

#define STRIDE_LEVEL 1

#include "sve_speculative_9.c"

/* { dg-final { scan-tree-dump-times "unknown alignment, setting loop as first faulting" 8 "vect" } } */
/* { dg-final { scan-tree-dump-not "loop versioned for vectorization to enhance alignment" "vect" } } */
/* { dg-final { scan-tree-dump-not "Alignment of access forced using peeling" "vect" } } */
/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 4 "vect" } } */