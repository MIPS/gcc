/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve" } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve -msve-vector-bits=256" { target aarch64_sve256_hw } } */

/* Use exit condition of 0.  */
#define EXIT_CONDITION 0
#define FILL_DATA 1
#include "sve_speculative_5_run.c"
