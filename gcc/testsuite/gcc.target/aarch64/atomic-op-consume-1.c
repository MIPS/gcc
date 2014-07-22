/* { dg-do compile } */
/* { dg-skip-if "" { aarch64_atomic } } */
/* { dg-options "-O2 -march=armv8-a+noatomic" } */

#include "atomic-op-consume.x"

/* { dg-final { scan-assembler-times "ldxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 6 } } */
/* { dg-final { scan-assembler-times "stxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 6 } } */
