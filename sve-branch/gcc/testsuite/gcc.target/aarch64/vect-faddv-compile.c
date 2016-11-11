
/* { dg-do compile } */
/* { dg-options "-O3 -ffast-math -march=armv8-a" } */

#include "vect-faddv.x"

/* { dg-final { scan-assembler-times "faddp\\tv" 2} } */
