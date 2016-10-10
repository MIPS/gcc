/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -ffast-math -fno-inline -march=armv8-a+sve" } */

#include "sve_reduc_2.C"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NROWS 5

#define DEF_INIT_VECTOR(TYPE)					\
  TYPE mat_##TYPE[NROWS][NUM_ELEMS (TYPE)];			\
  TYPE r_##TYPE[NROWS];						\
  for (int i = 0; i < NROWS; i++)				\
    for (int j = 0; j < NUM_ELEMS (TYPE); j++ )			\
      mat_##TYPE[i][j] = i + (j * 2) * (j & 1 ? 1 : -1);

#define TEST_REDUC_PLUS(TYPE) reduc_plus_##TYPE (mat_##TYPE, r_##TYPE, NROWS);
#define TEST_REDUC_MAX(TYPE)  reduc_max##TYPE   (mat_##TYPE, r_##TYPE, NROWS);
#define TEST_REDUC_MIN(TYPE)  reduc_min##TYPE   (mat_##TYPE, r_##TYPE, NROWS);
#define TEST_REDUC_AND(TYPE)  reduc_and##TYPE   (mat_##TYPE, r_##TYPE, NROWS);
#define TEST_REDUC_IOR(TYPE)  reduc_ior##TYPE   (mat_##TYPE, r_##TYPE, NROWS);
#define TEST_REDUC_XOR(TYPE)  reduc_xor##TYPE   (mat_##TYPE, r_##TYPE, NROWS);

#define SUM_VECTOR(RES, TYPE)\
  for (int i = 0; i < NROWS; i++)\
    (RES) += r_##TYPE[i];

#define SUM_INT_RESULT(RES)\
  SUM_VECTOR (RES, int8_t);\
  SUM_VECTOR (RES, int16_t);\
  SUM_VECTOR (RES, int32_t);\
  SUM_VECTOR (RES, int64_t);\
  SUM_VECTOR (RES, uint8_t);\
  SUM_VECTOR (RES, uint16_t);\
  SUM_VECTOR (RES, uint32_t);\
  SUM_VECTOR (RES, uint64_t);\

#define SUM_FLOAT_RESULT(RES)\
  SUM_VECTOR (RES, float);\
  SUM_VECTOR (RES, double);\

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

  TEST_REDUC_PLUS (int8_t)
  TEST_REDUC_PLUS (int16_t)
  TEST_REDUC_PLUS (int32_t)
  TEST_REDUC_PLUS (int64_t)
  TEST_REDUC_PLUS (uint8_t)
  TEST_REDUC_PLUS (uint16_t)
  TEST_REDUC_PLUS (uint32_t)
  TEST_REDUC_PLUS (uint64_t)
  TEST_REDUC_PLUS (float)
  TEST_REDUC_PLUS (double)

  SUM_INT_RESULT (result);
  SUM_FLOAT_RESULT (resultF);

  TEST_REDUC_MIN (int8_t)
  TEST_REDUC_MIN (int16_t)
  TEST_REDUC_MIN (int32_t)
  TEST_REDUC_MIN (int64_t)
  TEST_REDUC_MIN (uint8_t)
  TEST_REDUC_MIN (uint16_t)
  TEST_REDUC_MIN (uint32_t)
  TEST_REDUC_MIN (uint64_t)
  TEST_REDUC_MIN (float)
  TEST_REDUC_MIN (double)

  TEST_REDUC_MAX (int8_t)
  TEST_REDUC_MAX (int16_t)
  TEST_REDUC_MAX (int32_t)
  TEST_REDUC_MAX (int64_t)
  TEST_REDUC_MAX (uint8_t)
  TEST_REDUC_MAX (uint16_t)
  TEST_REDUC_MAX (uint32_t)
  TEST_REDUC_MAX (uint64_t)
  TEST_REDUC_MAX (float)
  TEST_REDUC_MAX (double)

  TEST_REDUC_AND (int8_t)
  TEST_REDUC_AND (int16_t)
  TEST_REDUC_AND (int32_t)
  TEST_REDUC_AND (int64_t)
  TEST_REDUC_AND (uint8_t)
  TEST_REDUC_AND (uint16_t)
  TEST_REDUC_AND (uint32_t)
  TEST_REDUC_AND (uint64_t)

  TEST_REDUC_IOR (int8_t)
  TEST_REDUC_IOR (int16_t)
  TEST_REDUC_IOR (int32_t)
  TEST_REDUC_IOR (int64_t)
  TEST_REDUC_IOR (uint8_t)
  TEST_REDUC_IOR (uint16_t)
  TEST_REDUC_IOR (uint32_t)
  TEST_REDUC_IOR (uint64_t)

  TEST_REDUC_XOR (int8_t)
  TEST_REDUC_XOR (int16_t)
  TEST_REDUC_XOR (int32_t)
  TEST_REDUC_XOR (int64_t)
  TEST_REDUC_XOR (uint8_t)
  TEST_REDUC_XOR (uint16_t)
  TEST_REDUC_XOR (uint32_t)
  TEST_REDUC_XOR (uint64_t)

  if (result != 26880)
    {
      fprintf (stderr, "result = %d\n", result);
      abort ();
    }

  if (resultF != double (5760))
    {
      fprintf (stderr, "resultF = %1.16lf\n", resultF);
      abort ();
    }

  return 0;
}
