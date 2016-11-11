/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -ffast-math -fno-inline -march=armv8-a+sve" } */

#include "sve_reduc_1.C"

#include <stdlib.h>
#include <stdio.h>

#define DEF_INIT_VECTOR(TYPE)				\
  TYPE r_##TYPE[NUM_ELEMS (TYPE) + 1];			\
  for (int i = 0; i < NUM_ELEMS (TYPE) + 1; i++)	\
    r_##TYPE[i] = (i * 2) * (i & 1 ? 1 : -1);

#define TEST_REDUC_PLUS(RES,TYPE) (RES) += reduc_plus_##TYPE (r_##TYPE);
#define TEST_REDUC_MAX(RES,TYPE) (RES) += reduc_max##TYPE (r_##TYPE);
#define TEST_REDUC_MIN(RES,TYPE) (RES) += reduc_min##TYPE (r_##TYPE);
#define TEST_REDUC_AND(RES,TYPE) (RES) += reduc_and##TYPE (r_##TYPE);
#define TEST_REDUC_IOR(RES,TYPE) (RES) += reduc_ior##TYPE (r_##TYPE);
#define TEST_REDUC_XOR(RES,TYPE) (RES) += reduc_xor##TYPE (r_##TYPE);

int main ()
{
  int result = 0;
  double resultF = 0.0;
  DEF_INIT_VECTOR (int8_t)
  DEF_INIT_VECTOR (int16_t)
  DEF_INIT_VECTOR (int32_t)
  DEF_INIT_VECTOR (int64_t)
  DEF_INIT_VECTOR (uint8_t)
  DEF_INIT_VECTOR (uint16_t)
  DEF_INIT_VECTOR (uint32_t)
  DEF_INIT_VECTOR (uint64_t)
  DEF_INIT_VECTOR (float)
  DEF_INIT_VECTOR (double)

  TEST_REDUC_PLUS (result, int8_t)
  TEST_REDUC_PLUS (result, int16_t)
  TEST_REDUC_PLUS (result, int32_t)
  TEST_REDUC_PLUS (result, int64_t)
  TEST_REDUC_PLUS (result, uint8_t)
  TEST_REDUC_PLUS (result, uint16_t)
  TEST_REDUC_PLUS (result, uint32_t)
  TEST_REDUC_PLUS (result, uint64_t)
  TEST_REDUC_PLUS (resultF, float)
  TEST_REDUC_PLUS (resultF, double)

  TEST_REDUC_MIN (result, int8_t)
  TEST_REDUC_MIN (result, int16_t)
  TEST_REDUC_MIN (result, int32_t)
  TEST_REDUC_MIN (result, int64_t)
  TEST_REDUC_MIN (result, uint8_t)
  TEST_REDUC_MIN (result, uint16_t)
  TEST_REDUC_MIN (result, uint32_t)
  TEST_REDUC_MIN (result, uint64_t)
  TEST_REDUC_MIN (resultF, float)
  TEST_REDUC_MIN (resultF, double)

  TEST_REDUC_MAX (result, int8_t)
  TEST_REDUC_MAX (result, int16_t)
  TEST_REDUC_MAX (result, int32_t)
  TEST_REDUC_MAX (result, int64_t)
  TEST_REDUC_MAX (result, uint8_t)
  TEST_REDUC_MAX (result, uint16_t)
  TEST_REDUC_MAX (result, uint32_t)
  TEST_REDUC_MAX (result, uint64_t)
  TEST_REDUC_MAX (resultF, float)
  TEST_REDUC_MAX (resultF, double)

  TEST_REDUC_AND (result, int8_t)
  TEST_REDUC_AND (result, int16_t)
  TEST_REDUC_AND (result, int32_t)
  TEST_REDUC_AND (result, int64_t)
  TEST_REDUC_AND (result, uint8_t)
  TEST_REDUC_AND (result, uint16_t)
  TEST_REDUC_AND (result, uint32_t)
  TEST_REDUC_AND (result, uint64_t)

  TEST_REDUC_IOR (result, int8_t)
  TEST_REDUC_IOR (result, int16_t)
  TEST_REDUC_IOR (result, int32_t)
  TEST_REDUC_IOR (result, int64_t)
  TEST_REDUC_IOR (result, uint8_t)
  TEST_REDUC_IOR (result, uint16_t)
  TEST_REDUC_IOR (result, uint32_t)
  TEST_REDUC_IOR (result, uint64_t)

  TEST_REDUC_XOR (result, int8_t)
  TEST_REDUC_XOR (result, int16_t)
  TEST_REDUC_XOR (result, int32_t)
  TEST_REDUC_XOR (result, int64_t)
  TEST_REDUC_XOR (result, uint8_t)
  TEST_REDUC_XOR (result, uint16_t)
  TEST_REDUC_XOR (result, uint32_t)
  TEST_REDUC_XOR (result, uint64_t)

  if (result != 262400)
    {
      fprintf (stderr, "result = %d\n", result);
      abort ();
    }

  if (resultF != -160)
    {
      fprintf (stderr, "resultF = %1.16lf\n", resultF);
      abort ();
    }

  return 0;
}
