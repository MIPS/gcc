/* { dg-do run { target { ! *-*-* } } } */
/* { dg-options "-std=c++11 -O3 -fno-inline -march=armv8-a+sve" } */

#include "sve_vcond_1.C"

#include <stdlib.h>

#define TEST_VCOND(TYPE,COND,SUFFIX)			\
{							\
  TYPE x = { 1 }, y = { 2 }, a = { 3 }, b = { 4 };	\
  r_##TYPE += vcond_##TYPE##SUFFIX (x, y, a, b);	\
}

#define TEST_VCOND_IMM(TYPE,COND,IMM,SUFFIX)		\
{							\
  TYPE x = { 1 }, y = { 2 }, a = { 3 };			\
  r_##TYPE += vcond_imm_##TYPE##SUFFIX (x, y, a);	\
}


#define TEST_VCOND_SIGNED_ALL(COND, SUFFIX)	\
TEST_VCOND (v32qi, COND, SUFFIX)		\
TEST_VCOND (v16hi, COND, SUFFIX)		\
TEST_VCOND (v8si, COND, SUFFIX)			\
TEST_VCOND (v4di, COND, SUFFIX)

#define TEST_VCOND_UNSIGNED_ALL(COND, SUFFIX)	\
TEST_VCOND (v32qu, COND, SUFFIX)		\
TEST_VCOND (v16hu, COND, SUFFIX)		\
TEST_VCOND (v8su, COND, SUFFIX)			\
TEST_VCOND (v4du, COND, SUFFIX)

#define TEST_VCOND_ALL(COND, SUFFIX)		\
TEST_VCOND_SIGNED_ALL (COND, SUFFIX)		\
TEST_VCOND_UNSIGNED_ALL(COND, SUFFIX)

#define TEST_VCOND_IMM_SIGNED_ALL(COND, IMM, SUFFIX)	\
TEST_VCOND_IMM (v32qi, COND, IMM, SUFFIX)		\
TEST_VCOND_IMM (v16hi, COND, IMM, SUFFIX)		\
TEST_VCOND_IMM (v8si, COND, IMM, SUFFIX)		\
TEST_VCOND_IMM (v4di, COND, IMM, SUFFIX)

#define TEST_VCOND_IMM_UNSIGNED_ALL(COND, IMM, SUFFIX)	\
TEST_VCOND_IMM (v32qu, COND, IMM, SUFFIX)		\
TEST_VCOND_IMM (v16hu, COND, IMM, SUFFIX)		\
TEST_VCOND_IMM (v8su, COND, IMM, SUFFIX)		\
TEST_VCOND_IMM (v4du, COND, IMM, SUFFIX)

#define TEST_VCOND_IMM_ALL(COND,IMM,SUFFIX)	\
TEST_VCOND_IMM_SIGNED_ALL (COND,IMM,SUFFIX)	\
TEST_VCOND_IMM_UNSIGNED_ALL (COND,IMM,SUFFIX)

#define DEF_INIT_VECTOR(TYPE)			\
  TYPE r_##TYPE;				\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )	\
    r_##TYPE[i] = i * 3;

#define SUM_VECTOR(VAL,TYPE)			\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )	\
    VAL += r_##TYPE[i];

int main (int argc, char **argv)
{
  int result = 0;
  DEF_INIT_VECTOR (v32qi)
  DEF_INIT_VECTOR (v16hi)
  DEF_INIT_VECTOR (v8si)
  DEF_INIT_VECTOR (v4di)
  DEF_INIT_VECTOR (v32qu)
  DEF_INIT_VECTOR (v16hu)
  DEF_INIT_VECTOR (v8su)
  DEF_INIT_VECTOR (v4du)

  TEST_VCOND_ALL (>, _gt)
  TEST_VCOND_ALL (<, _lt)
  TEST_VCOND_ALL (>=, _ge)
  TEST_VCOND_ALL (<=, _le)
  TEST_VCOND_ALL (==, _eq)
  TEST_VCOND_ALL (!=, _ne)

  TEST_VCOND_IMM_ALL (>, 5, _gt)
  TEST_VCOND_IMM_ALL (<, 5, _lt)
  TEST_VCOND_IMM_ALL (>=, 5, _ge)
  TEST_VCOND_IMM_ALL (<=, 5, _le)
  TEST_VCOND_IMM_ALL (==, 5, _eq)
  TEST_VCOND_IMM_ALL (!=, 5, _ne)

  SUM_VECTOR (result, v32qi)
  SUM_VECTOR (result, v16hi)
  SUM_VECTOR (result, v8si)
  SUM_VECTOR (result, v4di)
  SUM_VECTOR (result, v32qu)
  SUM_VECTOR (result, v16hu)
  SUM_VECTOR (result, v8su)
  SUM_VECTOR (result, v4du)

  if (result != 4044)
    abort ();
  return 0;
}
