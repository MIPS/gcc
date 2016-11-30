/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve" } */

#define STRIDE_LEVEL 2
#define EXIT_CONDITION 17
#define LOOP_COUNTS {21,7,9,85,24,31}

#include "sve_speculative_9_run.c"
