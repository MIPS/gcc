/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -fno-trapping-math" } */
/* { dg-require-effective-target fenv_exceptions } */

#define TEST_EXCEPTIONS 0
#include "sve_vcond_4_run.c"
