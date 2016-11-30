/* { dg-do compile } */
/* { dg-options "-O3 -fno-inline -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with a load and a test.  */

uint32_t
search (uint32_t *array)
{
  for (;;)
    {
      uint32_t x = *array++ >> 7;
      if (x >= 200)
        return x;
    }
}

/* { dg-final { scan-tree-dump "LOOP VECTORIZED" "vect" } } */
/* { dg-final { scan-assembler-times {\tbrka\tp[0-9]+\.b, p[0-9]+/z, p[0-9]+\.b} 1 } } */
/* { dg-final { scan-assembler-times {\tlastb\tw[0-9]+, p[0-7], z[0-9]+\.s} 1 } } */
