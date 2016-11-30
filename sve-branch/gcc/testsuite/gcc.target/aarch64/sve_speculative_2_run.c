/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-inline -march=armv8-a+sve -ffast-math -msve-vector-bits=256" } */

#include "sve_speculative_2.c"

extern void abort (void);

#define TEST_LOOP(ARGTYPE,INDUCTYPE)\
{\
  INDUCTYPE res = spec_loop_##ARGTYPE##INDUCTYPE (0xFF, 0xAE, true);\
  if (res != 0xAE)\
    abort ();\
}\

#define TEST_FP_LOOP(ARGTYPE,INDUCTYPE,FPTYPE)\
{\
  FPTYPE res = spec_fp_loop_##ARGTYPE##INDUCTYPE##FPTYPE (0xFF, 0xAE, true);\
  if (res != 0xAE)\
    abort ();\
}\

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

  TEST_FP_LOOP (uint32_t, uint32_t, float)
  TEST_FP_LOOP (uint64_t, uint64_t, double)

  return 0;
}
