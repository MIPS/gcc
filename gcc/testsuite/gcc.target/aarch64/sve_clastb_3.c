/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE uint8_t

#include "sve_clastb_2.c"

/* { dg-final { scan-assembler {\tclastb\tw[0-9]+, p[0-7]+, w[0-9]+, z[0-9]+\.b} } } */
