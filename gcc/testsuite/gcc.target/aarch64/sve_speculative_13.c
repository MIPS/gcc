/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

/* Speculative loop with two loads from global buffers which can be aligned
   without any peeling, and an access stride of 3.  */

#define STRIDE_LEVEL 3

#include "sve_speculative_11.c"

/* { dg-final { scan-tree-dump-times "not vectorized: can't calculate required alignment for data ref" 10 "vect" } } */
/* { dg-final { scan-tree-dump-not "LOOP VECTORIZED" "vect" } } */
