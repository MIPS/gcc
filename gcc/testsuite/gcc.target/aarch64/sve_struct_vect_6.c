/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE double
#include "sve_struct_vect_1.c"

/* { dg-final { scan-assembler {\tld2d\t{z[0-9]+.d - z[0-9]+.d}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tld3d\t{z[0-9]+.d - z[0-9]+.d}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tld4d\t{z[0-9]+.d - z[0-9]+.d}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst2d\t{z[0-9]+.d - z[0-9]+.d}, p[0-7], \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst3d\t{z[0-9]+.d - z[0-9]+.d}, p[0-7], \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst4d\t{z[0-9]+.d - z[0-9]+.d}, p[0-7], \[x[0-9]+\]\n} } } */
