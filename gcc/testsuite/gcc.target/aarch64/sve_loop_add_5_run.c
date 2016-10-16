/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve -msve-vector-bits=256" } */

#include "sve_loop_add_4_run.c"
