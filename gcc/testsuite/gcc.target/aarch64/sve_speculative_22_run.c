/* { dg-do run { target { aarch64_sve_hw } } } */
/* { dg-options "-O3 -fno-common -ffast-math -march=armv8-a+sve" } */

#include "sve_speculative_22.c"

extern void abort (void);

#ifndef MAX_ARRAY_SIZE
#define MAX_ARRAY_SIZE 500
#endif

#ifndef FILL_DATA_A
#define FILL_DATA_A 55
#endif

#ifndef FILL_DATA_B
#define FILL_DATA_B 7
#endif

/* Fill the arrays with the exit conditions.  */

#define TEST_SPEC_LOOP_FUNC(DATATYPE, ARGTYPE)\
void test_spec_loop_##DATATYPE##_##ARGTYPE ()\
{\
  DATATYPE a[MAX_ARRAY_SIZE];\
  DATATYPE b[MAX_ARRAY_SIZE];\
  int i;\
  for (i=0; i<MAX_ARRAY_SIZE; i++)\
    {\
      a[i] = FILL_DATA_A;\
      b[i] = FILL_DATA_B;\
    }\
  ARGTYPE ret = spec_loop_##DATATYPE##_##ARGTYPE (a, b);\
  if (ret != FILL_DATA_A + FILL_DATA_B)\
    abort ();\
}

TEST_SPEC_LOOP_FUNC (int8_t, int8_t)
TEST_SPEC_LOOP_FUNC (int16_t, int16_t)
TEST_SPEC_LOOP_FUNC (int32_t, int32_t)
TEST_SPEC_LOOP_FUNC (int64_t, int64_t)
TEST_SPEC_LOOP_FUNC (float, int32_t)
TEST_SPEC_LOOP_FUNC (double, int64_t)

int main (void)
{
  test_spec_loop_int8_t_int8_t ();
  test_spec_loop_int16_t_int16_t ();
  test_spec_loop_int32_t_int32_t ();
  test_spec_loop_int64_t_int64_t ();
  test_spec_loop_float_int32_t ();
  test_spec_loop_double_int64_t ();
  return 0;
}
