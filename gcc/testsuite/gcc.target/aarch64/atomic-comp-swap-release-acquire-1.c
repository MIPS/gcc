/* { dg-do compile } */
/* { dg-skip-if "" { aarch64_atomic } } */
/* { dg-options "-O2 -march=armv8-a+noatomic" } */

#include "atomic-comp-swap-release-acquire.x"

/* { dg-final { scan-assembler-times "ldaxr\tw\[0-9\]+, \\\[x\[0-9\]+\\\]" 4 } } */
/* { dg-final { scan-assembler-times "stlxr\tw\[0-9\]+, w\[0-9\]+, \\\[x\[0-9\]+\\\]" 4 } } */
