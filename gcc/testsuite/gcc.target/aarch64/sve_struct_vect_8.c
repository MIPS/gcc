/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE unsigned short
#define ITYPE short
#include "sve_struct_vect_7.c"

/* { dg-final { scan-assembler {\tld2h\t{z[0-9]+.h - z[0-9]+.h}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tld3h\t{z[0-9]+.h - z[0-9]+.h}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tld4h\t{z[0-9]+.h - z[0-9]+.h}, p[0-7]/z, \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst2h\t{z[0-9]+.h - z[0-9]+.h}, p[0-7], \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst3h\t{z[0-9]+.h - z[0-9]+.h}, p[0-7], \[x[0-9]+\]\n} } } */
/* { dg-final { scan-assembler {\tst4h\t{z[0-9]+.h - z[0-9]+.h}, p[0-7], \[x[0-9]+\]\n} } } */
