/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define TYPE _Float16
#define ITYPE short
#include "sve_struct_vect_7_run.c"
