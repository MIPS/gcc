/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve" } */

#define STRIDE_LEVEL 3
#define EXIT_CONDITION 93
#define LOOP_COUNTS {93,7,82,39,76,55}

#include "sve_speculative_9_run.c"
