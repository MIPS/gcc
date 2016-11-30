/* { dg-do compile } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

/* Speculative loop with two loads from global buffers which can be aligned
   without any peeling, and an access stride of 4.  */

#define STRIDE_LEVEL 4

#include "sve_speculative_11.c"

/* { dg-final { scan-tree-dump-not "loop versioned for vectorization to enhance alignment" "vect" } } */
/* { dg-final { scan-tree-dump-not "Alignment of access forced using peeling" "vect" } } */

/* { dg-final { scan-tree-dump "force alignment of aint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "force alignment of bint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "force alignment of aint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "force alignment of bint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "force alignment of aint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "force alignment of bint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "force alignment of aint64_t" "vect" } } */
/* { dg-final { scan-tree-dump "force alignment of bint64_t" "vect" } } */

/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref bint8_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref bint16_t" "vect" { xfail *-*-* } } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref bint32_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref aint64_t" "vect" } } */
/* { dg-final { scan-tree-dump "misalign = 0 bytes of ref bint64_t" "vect" } } */

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 4 "vect" } } */
