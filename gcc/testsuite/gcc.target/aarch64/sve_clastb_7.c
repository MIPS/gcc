/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE double
#include "sve_clastb_6.c"

/* { dg-final { scan-assembler {\tclastb\td[0-9]+, p[0-7], d[0-9]+, z[0-9]+\.d} } } */
