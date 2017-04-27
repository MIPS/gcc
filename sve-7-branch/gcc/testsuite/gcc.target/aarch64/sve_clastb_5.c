/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -fno-inline -march=armv8-a+sve --save-temps -fdump-tree-vect-details" } */

#define TYPE long

#include "sve_clastb_2.c"

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 1 "vect" } } */
/* { dg-final { scan-tree-dump-not "condition expression based on integer induction." "vect" } } */
/* { dg-final { scan-tree-dump-times "Optimizing condition reduction with CLASTB." 2 "vect" } } */
/* { dg-final { scan-assembler {\tclastb\tx[0-9]+, p[0-7], x[0-9]+, z[0-9]+\.d} } } */
