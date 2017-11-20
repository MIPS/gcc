/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE double
#include "sve_struct_vect_1_run.c"
