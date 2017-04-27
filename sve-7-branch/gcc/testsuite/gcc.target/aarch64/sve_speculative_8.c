/* { dg-do compile } */
/* { dg-options "-O3 -fno-inline -ffast-math -march=armv8-a+sve -fdump-tree-vect-details -msve-vector-bits=256" } */

#include <stdint.h>

/* Speculative loop with a load which requires multiple copies and a test.  */

uint32_t
search (uint64_t *array)
{
  for (;;)
    {
      uint32_t x = *array++ >> 7;
      if (x >= 200)
        return x;
    }
}

/* { dg-final { scan-tree-dump "multiple copies not supported for speculative loops" "vect" } } */
/* { dg-final { scan-tree-dump "not vectorized: relevant stmt not supported" "vect" } } */
/* { dg-final { scan-tree-dump-not "LOOP VECTORIZED" "vect" } } */
