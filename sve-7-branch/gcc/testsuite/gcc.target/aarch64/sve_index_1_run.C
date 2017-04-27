/* { dg-do run { target aarch64_sve_hw } } */
/* { dg-options "-std=c++11 -O2 -ftree-vectorize -fno-inline -march=armv8-a+sve" } */

#include "sve_index_1.C"

#include <stdlib.h>
#include <stdio.h>

#define SUM_VECTOR(TYPE)			\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++ )	\
  {						\
    result += r_##TYPE[i];			\
  }

#define TEST_LOOPS_ALL_UNSIGNED_TYPES(SUFFIX)	\
loop_uint8_t##SUFFIX (r_uint8_t);		\
loop_uint16_t##SUFFIX (r_uint16_t);		\
loop_uint32_t##SUFFIX (r_uint32_t);		\
loop_uint64_t##SUFFIX (r_uint64_t);		\
SUM_VECTOR (uint8_t);				\
SUM_VECTOR (uint16_t);				\
SUM_VECTOR (uint32_t);				\
SUM_VECTOR (uint64_t);

#define TEST_LOOPS_ALL_SIGNED_TYPES(SUFFIX)	\
loop_int8_t##SUFFIX (r_int8_t);			\
loop_int16_t##SUFFIX (r_int16_t);		\
loop_int32_t##SUFFIX (r_int32_t);		\
loop_int64_t##SUFFIX (r_int64_t);		\
SUM_VECTOR (int8_t);				\
SUM_VECTOR (int16_t);				\
SUM_VECTOR (int32_t);				\
SUM_VECTOR (int64_t);


#define DEF_INIT_VECTOR(TYPE)			\
  TYPE r_##TYPE[NUM_ELEMS (TYPE)];		\
  for (int i = 0; i < NUM_ELEMS (TYPE); i++)	\
    r_##TYPE[i] = 0;

int main ()
{
  int result = 0;
  DEF_INIT_VECTOR (int8_t)
  DEF_INIT_VECTOR (int16_t)
  DEF_INIT_VECTOR (int32_t)
  DEF_INIT_VECTOR (int64_t)
  DEF_INIT_VECTOR (uint8_t)
  DEF_INIT_VECTOR (uint16_t)
  DEF_INIT_VECTOR (uint32_t)
  DEF_INIT_VECTOR (uint64_t)

  TEST_LOOPS_ALL_UNSIGNED_TYPES (b0s1)
  TEST_LOOPS_ALL_SIGNED_TYPES (b0s1)
  TEST_LOOPS_ALL_UNSIGNED_TYPES (b0s15)
  TEST_LOOPS_ALL_SIGNED_TYPES (b0s15)
  TEST_LOOPS_ALL_SIGNED_TYPES (b0sm1)
  TEST_LOOPS_ALL_SIGNED_TYPES (b0sm16)
  TEST_LOOPS_ALL_SIGNED_TYPES (bm16s1)
  TEST_LOOPS_ALL_UNSIGNED_TYPES (b15s1)
  TEST_LOOPS_ALL_SIGNED_TYPES (b15s1)

  TEST_LOOPS_ALL_UNSIGNED_TYPES (b0s16)
  TEST_LOOPS_ALL_SIGNED_TYPES (b0s16)
  TEST_LOOPS_ALL_SIGNED_TYPES (b0sm17)
  TEST_LOOPS_ALL_SIGNED_TYPES (bm17s1)
  TEST_LOOPS_ALL_UNSIGNED_TYPES (b16s1)
  TEST_LOOPS_ALL_SIGNED_TYPES (b16s1)
  TEST_LOOPS_ALL_UNSIGNED_TYPES (b16s16)
  TEST_LOOPS_ALL_SIGNED_TYPES (b16s16)
  TEST_LOOPS_ALL_SIGNED_TYPES (bm17sm17)

  if (result != 24270)
    {
      fprintf (stderr, "result = %d\n", result);
      abort ();
    }
  return 0;
}
