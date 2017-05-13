/* { dg-do compile } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256" } */

#include "sve_speculative_24.c"

/* { dg-final { scan-assembler {\tld1w\t.*\tbrka\t.*\tadd\tz[0-9]\.s, p[0-7]/m,} } } */
/* { dg-final { scan-assembler {\twhilelo\t} } } */
