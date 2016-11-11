/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -ffast-math -fno-inline -march=armv8-a+sve" } */

#include "sve_maxmin_1.C"

#include <stdlib.h>
#include <stdio.h>

#define DEF_INIT_VECTOR(TYPE)				\
  TYPE a_##TYPE[NUM_ELEMS (TYPE)];			\
  TYPE b_##TYPE[NUM_ELEMS (TYPE)];			\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )		\
  {							\
    a_##TYPE[i] = ((i * 2) % 3) * (i & 1 ? 1 : -1);	\
    b_##TYPE[i] = (1 + (i % 4)) * (i & 1 ? -1 : 1);	\
  }

#define TEST_MAX(RES,TYPE)			\
{						\
  TYPE r_##TYPE[NUM_ELEMS (TYPE)];		\
  fun_max##TYPE (r_##TYPE, a_##TYPE, b_##TYPE);	\
  TYPE tmp = 0;					\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    tmp += r_##TYPE[i];				\
  (RES) += tmp;					\
}

#define TEST_MIN(RES,TYPE)			\
{						\
  TYPE r_##TYPE[NUM_ELEMS (TYPE)];		\
  fun_max##TYPE (r_##TYPE, a_##TYPE, b_##TYPE);	\
  TYPE tmp = 0;					\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    tmp += r_##TYPE[i];				\
  (RES) += tmp;					\
}

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

  TEST_MIN (result, int8_t)
  TEST_MIN (result, int16_t)
  TEST_MIN (result, int32_t)
  TEST_MIN (result, int64_t)
  TEST_MIN (result, uint8_t)
  TEST_MIN (result, uint16_t)
  TEST_MIN (result, uint32_t)
  TEST_MIN (result, uint64_t)
  TEST_MIN (resultF, float)
  TEST_MIN (resultF, double)

  TEST_MAX (result, int8_t)
  TEST_MAX (result, int16_t)
  TEST_MAX (result, int32_t)
  TEST_MAX (result, int64_t)
  TEST_MAX (result, uint8_t)
  TEST_MAX (result, uint16_t)
  TEST_MAX (result, uint32_t)
  TEST_MAX (result, uint64_t)
  TEST_MAX (resultF, float)
  TEST_MAX (resultF, double)

  if (result != 131400)
    {
      fprintf (stderr, "result = %d\n", result);
      abort ();
    }

  if (resultF != 362)
    {
      fprintf (stderr, "resultF = %1.16lf\n", resultF);
      abort ();
    }

  return 0;
}
