/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -msve-vector-bits=scalable --save-temps" } */

#include "sve_strided_load_5.c"

/* { dg-final { scan-assembler-not {\[x[0-9]+, z[0-9]+\.s} } } */
/* { dg-final { scan-assembler-times {\tld1d\tz[0-9]+\.d, p[0-7]/z, \[x[0-9]+, z[0-9]+.d\]\n} 15 } } */
