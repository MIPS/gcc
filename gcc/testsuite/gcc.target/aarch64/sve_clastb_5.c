/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE uint64_t

#include "sve_clastb_2.c"

/* { dg-final { scan-assembler {\tclastb\tx[0-9]+, p[0-7], x[0-9]+, z[0-9]+\.d} } } */
