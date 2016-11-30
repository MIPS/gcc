/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve -ffast-math -msve-vector-bits=256" } */

#include "sve_speculative_4.c"

extern void abort (void);
#include <string.h>

#define MAX 0xAE

#define TEST_LOOP(ARGTYPE,INDUCTYPE)\
{\
  ARGTYPE array[MAX];\
  memset (array, 0, sizeof (ARGTYPE) * MAX);\
  array[MAX - 1] = 72;\
  INDUCTYPE res = spec_loop_##ARGTYPE##INDUCTYPE (0xFF, MAX, array);\
  if (res != 72)\
    abort ();\
}

#define TEST_FP_LOOP(ARGTYPE,FPTYPE)\
{\
  FPTYPE array[MAX];\
  memset (array, 0, sizeof (FPTYPE) * MAX);\
  array[MAX - 1] = 54.5;\
  FPTYPE res = spec_fp_loop_##ARGTYPE##FPTYPE (0xFF, MAX, array);\
  if (res != 54.5)\
    abort ();\
}

int main ()
{
  TEST_LOOP (uint8_t, uint8_t);
  TEST_LOOP (uint16_t, uint16_t);
  TEST_LOOP (uint32_t, uint32_t);
  TEST_LOOP (uint64_t, uint64_t);
  TEST_LOOP (int32_t, int32_t);
  TEST_LOOP (int64_t, int64_t);

  TEST_LOOP (uint16_t, uint8_t)

  TEST_LOOP (uint32_t, uint8_t)
  TEST_LOOP (uint32_t, uint16_t)

  TEST_LOOP (uint64_t, uint8_t)
  TEST_LOOP (uint64_t, uint16_t)
  TEST_LOOP (uint64_t, uint32_t)

  TEST_FP_LOOP (uint32_t, float)
  TEST_FP_LOOP (uint64_t, double)

  TEST_FP_LOOP (uint64_t, float)

  return 0;
}
