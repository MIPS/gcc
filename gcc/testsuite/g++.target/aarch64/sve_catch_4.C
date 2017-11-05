/* { dg-do run } */
/* { dg-options "-O3 -fopenmp-simd -fomit-frame-pointer" } */
/* { dg-options "-O3 -fopenmp-simd -fomit-frame-pointer -march=armv8-a+sve" { target aarch64_sve_hw } } */

#include "sve_catch_3.C"
