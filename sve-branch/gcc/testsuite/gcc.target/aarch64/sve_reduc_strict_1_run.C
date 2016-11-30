/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include "sve_reduc_strict_1.C"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define DEF_INIT_VECTOR(TYPE)\
  TYPE a_##TYPE[NUM_ELEMS (TYPE)];\
  TYPE b_##TYPE[NUM_ELEMS (TYPE)];\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )\
    {\
      a_##TYPE[i] = (i * 2) * (i & 1 ? 1 : -1);\
      b_##TYPE[i] = (i * 3) * (i & 1 ? 1 : -1);\
    }

#define TEST_REDUC_PLUS(RES,TYPE) (RES) += reduc_plus_##TYPE (a_##TYPE, b_##TYPE);
#define TEST_REDUC_MAX(RES,TYPE) (RES) += reduc_fmax (a_##TYPE, b_##TYPE);
#define TEST_REDUC_MAXF(RES,TYPE) (RES) += reduc_fmaxf (a_##TYPE, b_##TYPE);
#define TEST_REDUC_MIN(RES,TYPE) (RES) += reduc_fmin (a_##TYPE, b_##TYPE);
#define TEST_REDUC_MINF(RES,TYPE) (RES) += reduc_fminf (a_##TYPE, b_##TYPE);

int main ()
{
  double result = 0.0;
  DEF_INIT_VECTOR (float)
  DEF_INIT_VECTOR (double)

  TEST_REDUC_PLUS (result, float)
  TEST_REDUC_PLUS (result, double)

  TEST_REDUC_MINF (result, float)
  TEST_REDUC_MIN (result, double)

  TEST_REDUC_MAXF (result, float)
  TEST_REDUC_MAX (result, double)

  if (result != double (1356996))
  {
    fprintf (stderr, "result = %1.16lf\n", result);
    abort ();
  }

  return 0;
}

