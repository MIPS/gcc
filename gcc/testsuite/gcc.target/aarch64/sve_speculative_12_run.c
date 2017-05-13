/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve -msve-vector-bits=256" } */

#define STRIDE_LEVEL 2
#define EXIT_CONDITION 7
#define LOOP_COUNTS {43,27,19,54,25,27}

#include "sve_speculative_11_run.c"
