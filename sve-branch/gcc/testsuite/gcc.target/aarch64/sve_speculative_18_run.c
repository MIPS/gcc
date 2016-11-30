/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve" } */

#define STRIDE_LEVEL 1
#define EXIT_CONDITION 97
#define LOOP_COUNTS {63,73,7,73,28,37}

#include "sve_speculative_9_run.c"
