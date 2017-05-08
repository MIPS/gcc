/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include <stdlib.h>
#include <stdio.h>
#include "sve_maxmin_strict_1.C"

#define DEF_INIT_VECTOR(TYPE)				\
  TYPE a_##TYPE[NUM_ELEMS (TYPE)];			\
  TYPE b_##TYPE[NUM_ELEMS (TYPE)];			\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )		\
  {							\
    a_##TYPE[i] = ((i * 2) % 3) * (i & 1 ? 1 : -1);	\
    b_##TYPE[i] = (1 + (i % 4)) * (i & 1 ? -1 : 1);	\
  }

#define TEST_MAX(RES,FUN,TYPE)			\
{						\
  TYPE r_##TYPE[NUM_ELEMS (TYPE)];		\
  test_##FUN (r_##TYPE, a_##TYPE, b_##TYPE);	\
  TYPE tmp = 0;					\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    tmp += r_##TYPE[i];				\
  (RES) += tmp;					\
}

#define TEST_MIN(RES,FUN,TYPE)			\
{						\
  TYPE r_##TYPE[NUM_ELEMS (TYPE)];		\
  test_##FUN (r_##TYPE, a_##TYPE, b_##TYPE);	\
  TYPE tmp = 0;					\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    tmp += r_##TYPE[i];				\
  (RES) += tmp;					\
}

int main ()
{
  double resultF = 0.0;
  DEF_INIT_VECTOR (float)
  DEF_INIT_VECTOR (double)

  TEST_MIN (resultF, fminf, float)
  TEST_MIN (resultF, fmin, double)

  TEST_MAX (resultF, fmaxf, float)
  TEST_MAX (resultF, fmax, double)

  if (resultF != -57)
    {
      fprintf (stderr, "resultF = %1.16lf\n", resultF);
      abort ();
    }

  return 0;
}
