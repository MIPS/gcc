/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -march=armv8-a+sve -msve-vector-bits=256" } */

#include "sve_speculative_24_run.c"
