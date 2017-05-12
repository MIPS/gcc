/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include "sve_reduc_strict_2.C"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NROWS 5

#define DEF_INIT_VECTOR(TYPE)\
  TYPE mat_##TYPE[NROWS][NUM_ELEMS (TYPE)];\
  TYPE r_##TYPE[NROWS];\
  for (int i = 0; i < NROWS; i++)\
    for (int j = 0; j < NUM_ELEMS (TYPE); j++ )\
      mat_##TYPE[i][j] = i + (j * 2) * (j & 1 ? 1 : -1);\

#define TEST_REDUC_PLUS(TYPE) reduc_plus_##TYPE (mat_##TYPE, r_##TYPE, NROWS);
#define TEST_REDUC_MAXF reduc_fmaxf (mat_float,  r_float,  NROWS);
#define TEST_REDUC_MAX  reduc_fmax  (mat_double, r_double, NROWS);
#define TEST_REDUC_MINF reduc_fminf (mat_float,  r_float,  NROWS);
#define TEST_REDUC_MIN  reduc_fmin  (mat_double, r_double, NROWS);

#define SUM_VECTOR(RES, TYPE)\
  for (int i = 0; i < NROWS; i++)\
    (RES) += r_##TYPE[i];

#define SUM_FLOAT_RESULT(RES)\
  SUM_VECTOR (RES, float);\
  SUM_VECTOR (RES, double);\

int main ()
{
  double resultF = 0.0;
  DEF_INIT_VECTOR (float)
  DEF_INIT_VECTOR (double)

  TEST_REDUC_PLUS (float)
  TEST_REDUC_PLUS (double)
  SUM_FLOAT_RESULT (resultF);

  TEST_REDUC_MAXF
  TEST_REDUC_MAX
  SUM_FLOAT_RESULT (resultF);

  TEST_REDUC_MINF
  TEST_REDUC_MIN
  SUM_FLOAT_RESULT (resultF);

  if (resultF != double (2460))
  {
    fprintf (stderr, "resultF = %1.16lf\n", resultF);
    abort ();
  }

  return 0;
}


