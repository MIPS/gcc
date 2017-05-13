/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve -msve-vector-bits=256" } */

/* Use exit condition of 0 and less than a single iteration.  */
#define EXIT_CONDITION 0
#define FILL_DATA 1
#define LOOP_COUNTS {3,5,3,1,5,1}
#include "sve_speculative_5_run.c"
